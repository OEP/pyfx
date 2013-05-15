#!/usr/bin/env python

#
# Criteria:
#  1. 25 implicit shapes
#  2. Includes: sphere, ellipse, torus, cone, box, icosahedron, steiner patch
#  3. Union, Intersect, Cutout
#  4. 3 distinct colors
#

import math
import sys
import time

from vr.graph import VolumeGraph
from vr.vrend import *
from vr.tree import *

import vr.cmd as cmd
import vr.path as path
import vr.render as render

def tuple2color(t):
  return Color(t[0], t[1], t[2], t[3])

def emph(t, k=10):
  return constantProduct(t, k)

COLOR_CLEMSON_ORANGE = (0.965, 0.404, 0.2, 1)
COLOR_CLEMSON_PURPLE = (0.322, 0.176, 0.502, 1)

COLOR_FLESH = (1, 0.871, 0.631, 1)
COLOR_FLOWER = (1, 0.5, 0.5, 1)

COLOR_HAT = (1, 1, 0, 1)
COLOR_ACCENT = (0, 0, 1, 1)
COLOR_TIE = (1, 0, 0, 1)
COLOR_SHOES = (179./255, 152./255, 0, 1)

ICOS_SCALE = 0.15

## Tie parameters
TIE_LENGTH = 1.25
TIE_K = 0.75
TIE_THETA = math.pi/16
TIE_THICK = 0.1

MAN_HEIGHT = 6.0

PITCHFORK_LENGTH = MAN_HEIGHT * 2/3
PITCHFORK_RADIUS = 0.1

TORSO_HEIGHT = 0.25 * MAN_HEIGHT
TORSO_MAJOR = TORSO_HEIGHT / 2
TORSO_MINOR = TORSO_MAJOR / 3

LEG_LENGTH = 0.30 * MAN_HEIGHT
LEG_THICKNESS = LEG_LENGTH / 4
HINDLEG_LENGTH = 0.40 * LEG_LENGTH
FORELEG_LENGTH = 0.40 * LEG_LENGTH
FOOT_RADIUS = 0.2 * LEG_LENGTH

ARM_LENGTH = 0.75 * TORSO_HEIGHT 
ARM_THICKNESS = ARM_LENGTH / 4

FOREARM_LENGTH = 0.45 * ARM_LENGTH
HINDARM_LENGTH = 0.45 * ARM_LENGTH
HAND_RADIUS = 0.2 * ARM_LENGTH

PELVIS_RADIUS = 2
PELVIS_SCALE = 0.25
PELVIS_P = 10

RFARM_THETA = 0
LFARM_THETA = math.pi/6
RHARM_THETA = math.pi/3.5
LHARM_THETA = math.pi/6
LHLEG_THETA = -math.pi/3
RHLEG_THETA = math.pi/3
RFLEG_THETA = math.pi/2
LFLEG_THETA = -math.pi/2

HEAD_PCT = 1.0 / 12.0
HEAD_SIZE = MAN_HEIGHT * (HEAD_PCT - 0.01)
HEAD_HEIGHT = MAN_HEIGHT * HEAD_PCT

## Hat parameters
BRIM_RADIUS = HEAD_SIZE * 1.75
FLOWER_SCALE = (0.1 / 0.15) * HEAD_SIZE
ACCENT_THICKNESS = (0.02 / 0.15) * HEAD_SIZE
BRIM_THICKNESS = (0.01 / 0.15) * HEAD_SIZE


def create_head():
  if HEAD_SIZE > HEAD_HEIGHT:
    return (Ellipse, HEAD_SIZE, HEAD_HEIGHT, Vector(1, 0, 0))
  else:
    return (Ellipse, HEAD_HEIGHT, HEAD_SIZE, Vector(0, 1, 0))

def create_torso():
  return (Ellipse, TORSO_MAJOR, TORSO_MINOR, Vector(0, 1, 0))

def create_pelvis():
  return (SoftBox, PELVIS_RADIUS, PELVIS_P)

def create_hindarm(which=1):
  return (Translate, Vector(0, 0, HINDARM_LENGTH)*which,
    (Ellipse, HINDARM_LENGTH, ARM_THICKNESS/2, Vector(0, 0, 1)))

def create_hindleg(which=1):
  return (Translate, Vector(0, 0, HINDLEG_LENGTH)*which,
    (Ellipse, HINDLEG_LENGTH, LEG_THICKNESS/2, Vector(0, 0, 1)))

def create_foreleg(which=1):
  return (Translate, Vector(0, 0, FORELEG_LENGTH)*which,
    (Ellipse, FORELEG_LENGTH, LEG_THICKNESS/2, Vector(0, 0, 1)))

def create_forearm(which=1):
  return (Translate, Vector(0, 0, FOREARM_LENGTH)*which,
    (Ellipse, FOREARM_LENGTH, ARM_THICKNESS/2, Vector(0, 0, 1)))

def create_tie():
  dk = TIE_LENGTH * TIE_K
  d1k = TIE_LENGTH * (1 - TIE_K)
  ttheta = math.tan(TIE_THETA)
  alpha = math.atan((ttheta * dk) / d1k)
  major = (Cone, Vector(0,1,0), TIE_K * TIE_LENGTH, TIE_THETA)
  minor = (Translate, Vector(0, TIE_LENGTH, 0),
    (Cone, Vector(0,-1,0), (1-TIE_K) * TIE_LENGTH, alpha))

  majmin = unionAll(major, minor)

  plane1 = (Plane, Vector.AXIS_X, Vector(-TIE_THICK/2, 0, 0))
  plane2 = (Plane, Vector.AXIS_X * -1, Vector(TIE_THICK/2, 0, 0))
  tie = intersectAll(majmin, plane1, plane2)
  tie = (Mask, tie)

  tie = (vr.Translate, Vector(TORSO_MINOR, -TORSO_HEIGHT/2, 0), tie)

  return (tie, scalar2rgba(tie, COLOR_TIE))

def create_hand():
  return (Sphere, HAND_RADIUS)

def create_foot():
  return (ScalarProduct, (ScalarConstant, 2), (Scale, 0.35, (SoftBox, FOOT_RADIUS, 10)))

def right_shoulder_position():
  return Vector(0, -TORSO_HEIGHT/8 - TORSO_HEIGHT/4, 0)

def left_shoulder_position():
  return Vector(0, -TORSO_HEIGHT/8, 0)

def right_legsocket_position():
  return Vector(0, TORSO_HEIGHT/2, 0)

def left_legsocket_position():
  return Vector(0, TORSO_HEIGHT/2, 0)

def right_hindleg_direction():
  return (Vector.AXIS_Z * 1).rotate(Vector.AXIS_X, RHLEG_THETA)

def right_foreleg_direction():
  return (Vector.AXIS_Z * 1).rotate(Vector.AXIS_X, RFLEG_THETA)

def left_foreleg_direction():
  return (Vector.AXIS_Z * -1).rotate(Vector.AXIS_X, LFLEG_THETA)

def right_knee_position():
  return right_legsocket_position() + right_hindleg_direction() * \
    HINDLEG_LENGTH * 2 * 0.8

def left_hindleg_direction():
  return (Vector.AXIS_Z * -1).rotate(Vector.AXIS_X, LHLEG_THETA)

def left_knee_position():
  return left_legsocket_position() + left_hindleg_direction() * \
    HINDLEG_LENGTH * 2 * 0.8

def right_hindarm_direction():
  return (Vector.AXIS_Z * 1).rotate(Vector.AXIS_X, RHARM_THETA)

def left_hindarm_direction():
  return (Vector.AXIS_Z *-1).rotate(Vector.AXIS_X, LHARM_THETA)

def left_hindarm_position():
  return left_shoulder_position() + left_hindarm_direction() * \
    HINDARM_LENGTH * 2 * 0.8

def right_hindarm_position():
  return right_shoulder_position() + right_hindarm_direction() * \
    HINDARM_LENGTH * 2 * 0.8

def right_forearm_direction():
  return (Vector.AXIS_Z *  1).rotate(Vector.AXIS_X, RFARM_THETA)

def left_forearm_direction():
  return (Vector.AXIS_Z * -1).rotate(Vector.AXIS_X, LFARM_THETA)

def right_forearm_position():
  return right_hindarm_position() + right_forearm_direction() * \
    FOREARM_LENGTH * 2

def left_forearm_position():
  return left_hindarm_position() + left_forearm_direction() * \
    FOREARM_LENGTH * 2

def left_ankle_position():
  return left_knee_position() + left_foreleg_direction() * \
    FORELEG_LENGTH * 2

def right_ankle_position():
  return right_knee_position() + right_foreleg_direction() * \
    FORELEG_LENGTH * 2

def left_hand_position():
  return left_forearm_position() + left_forearm_direction() * \
    HAND_RADIUS

def right_hand_position():
  return right_forearm_position() + right_forearm_direction() * \
    HAND_RADIUS

def create_body(subset='all'):
  head = (Translate, Vector(0,-TORSO_HEIGHT/2-HEAD_HEIGHT/2,0), create_head())
  torso = create_torso()
  rharm = create_hindarm()
  lharm = create_hindarm(-1)
  rfarm = create_forearm()
  lfarm = create_forearm(-1)
  lhand = create_hand()
  rhand = create_hand()
  pelvis = create_pelvis()
  lhleg = create_hindleg(-1)
  rhleg = create_hindleg()
  lfleg = create_foreleg(-1)
  rfleg = create_foreleg(1)
  lfoot = create_foot()
  rfoot = create_foot()
  pelvis = (Scale, PELVIS_SCALE, pelvis)

  rfarm = (Rotate, Vector.AXIS_X, RFARM_THETA, rfarm)
  lfarm = (Rotate, Vector.AXIS_X, LFARM_THETA, lfarm)
  lharm = (Rotate, Vector.AXIS_X, LHARM_THETA, lharm)
  rharm = (Rotate, Vector.AXIS_X, RHARM_THETA, rharm)
  lhleg = (Rotate, Vector.AXIS_X, LHLEG_THETA, lhleg)
  rhleg = (Rotate, Vector.AXIS_X, RHLEG_THETA, rhleg)
  lfleg = (Rotate, Vector.AXIS_X, LFLEG_THETA, lfleg)
  rfleg = (Rotate, Vector.AXIS_X, RFLEG_THETA, rfleg)

  lhleg = translateAll(left_legsocket_position(), lhleg)
  rhleg = translateAll(right_legsocket_position(), rhleg)
  lharm = translateAll(left_shoulder_position(), lharm)
  rharm = translateAll(right_shoulder_position(), rharm)
  lfarm = translateAll(left_hindarm_position(), lfarm)
  rfarm = translateAll(right_hindarm_position(), rfarm)
  rhand = translateAll(right_forearm_position(), rhand)
  lhand = translateAll(left_forearm_position(), lhand)
  pelvis = translateAll(Vector(0, TORSO_MAJOR*0.9, 0), pelvis)
  rfleg = translateAll(right_knee_position(), rfleg)
  lfleg = translateAll(left_knee_position(), lfleg)
  lfoot = translateAll(left_ankle_position(), lfoot)
  rfoot = translateAll(right_ankle_position(), rfoot)

  lhand = (ScalarProduct, lhand, (ScalarConstant, 8))
  rhand = (ScalarProduct, rhand, (ScalarConstant, 8))

  subsets = {
    'all': (torso,head,lharm,rharm,lfarm,rfarm,lhand,rhand,pelvis,lhleg,
              rhleg,lfleg,rfleg,rfoot,lfoot),
    'shirt': (torso, lharm, rharm, lfarm, rfarm),
    'pants': (pelvis, rhleg, lfleg, rfleg, lhleg),
    'shoes': (lfoot, rfoot),
    'hands': (rhand, lhand),
    'head': (head,),
  }

  what = subsets[subset]

  body = ('humanoid_' + subset,) + unionAll(*what)

  return body

def create_shirt():
  density = create_body('shirt')
  color = (Translate, Vector(1, 0.2, 0), (vr.Scale, 0.5, Icosahedron))
  color = (ColorReplaceBox, grad2rgba(color), Box.NIL)
  color = mergeAll(MergeColor.MAX, color, (ColorConstant, Color(0.0,0,1,1)))
  color = scalar2rgba(density, color)
  return (density, color)

def create_pants():
  density = create_body('pants')
  return (density, None)

def create_shoes():
  density = create_body('shoes')
  color = scalar2rgba(density, COLOR_SHOES)
  return (density,color)

def create_hands():
  density = create_body('hands')
  color = scalar2rgba(density, COLOR_FLESH)
  return (density, color)

def create_head_densitycolor():
  density = create_body('head')
  color = scalar2rgba(density, COLOR_FLESH)
  return (density, color)

def create_hat_accent():
  return translateAll(Vector(0,0,-(BRIM_THICKNESS+ACCENT_THICKNESS/2)),
    (Mask,
      (Torus,
        HEAD_SIZE + ACCENT_THICKNESS,
        ACCENT_THICKNESS,
        Vector(0,0,1))))

def create_hat_brim():
  return cutoutAll(
    intersectAll(
      (Plane, Vector(0, 0, 1), Vector(0, 0, -BRIM_THICKNESS)),
      (Plane, Vector(0, 0, -1), Vector(0, 0, BRIM_THICKNESS)),
      (Sphere, BRIM_RADIUS)),
    (Sphere, HEAD_SIZE - 2*BRIM_THICKNESS))

def create_hat_bowl():
  return cutoutAll(
    intersectAll(
      (Sphere, HEAD_SIZE),
      (Plane, Vector(0,0,-1))),
    (Sphere, HEAD_SIZE - 2*BRIM_THICKNESS))

def create_flower():
  direction = Vector(1,1,0).unit()
  pos = direction * (HEAD_SIZE+ACCENT_THICKNESS) \
    - Vector.AXIS_Z * FLOWER_SCALE - Vector.AXIS_Z * BRIM_THICKNESS
  return (Mask,
    translateAll(pos,
      (Scale, FLOWER_SCALE, SteinerPatch)))

def create_hat():
  brim = create_hat_brim()
  bowl = create_hat_bowl()
  main = emph( unionAll(brim, bowl))
  accent = create_hat_accent()
  flower = create_flower()

  main, accent, flower = \
    rotateAll(Vector.AXIS_X, -math.pi/8, main, accent, flower)

  pos = left_hand_position()
  tdir = Vector.AXIS_Y.rotate(Vector.AXIS_X, -math.pi/8)
  pos = pos - tdir * 0.9*BRIM_RADIUS

  main, accent, flower = \
    translateAll(pos, main, accent, flower)

  density = unionAll(main, accent, flower)

  color = mergeAll(MergeColor.MAX,
    scalar2rgba(flower, COLOR_FLOWER),
    scalar2rgba(main, COLOR_HAT),
    scalar2rgba(accent, COLOR_ACCENT))

  return (density, color)

def create_pitchfork():
  handle = (vr.Cylinder, Vector(0,-1,0), PITCHFORK_RADIUS, PITCHFORK_LENGTH)
  end = (vr.Scale, ICOS_SCALE, Icosahedron)
  end = (vr.Cutout, end, (Plane, Vector(0,-1,0)))
  end = (Translate, Vector(0, -PITCHFORK_LENGTH/2, 0), end)

  pos = right_hand_position()
  handle, end = translateAll(pos, handle, end)

  p1 = Vector(0, -PITCHFORK_LENGTH/4, 0)
  p2 = Vector(0, PITCHFORK_LENGTH/2, 0)
  p1 = p1 + pos
  p2 = p2 + pos
  c1 = tuple2color(COLOR_CLEMSON_ORANGE)
  c2 = tuple2color(COLOR_CLEMSON_PURPLE)
  c1 = (ColorConstant, c1)
  c2 = (ColorConstant, c2)
  colorfield = (ColorRamp, p1, p2, c1, c2)

  density = (ScalarProduct, unionAll(handle, end), (ScalarConstant, 2))
  colorfield = scalar2rgba(density, colorfield)

  return (density, colorfield)

def create_humanoid():
  densities = []
  colors = []

  def register(tup):
    if not isinstance(tup, tuple): tup = tuple(tup)
    density = tup[0]
    densities.append(density)
    color = tup[1] if tup[1] != None else scalar2rgba(density)
    colors.append(color)

  def collapse():
    return (clampDensity(unionAll(*densities), 0.01),
      mergeAll(MergeColor.MAX, *colors))
 
  register(create_shirt())
  register(create_pants())
  register(create_shoes())
  register(create_hands())
  register(create_head_densitycolor())
  register(create_hat())
  register(create_tie())
  register(create_pitchfork())

  return collapse()

def main():
  (density, color) = create_humanoid()

  c = Camera()
  c.look(12,-2.0,0,0,0,0,0,1,0)

  parser = cmd.getopt_render(default_name='hw1')
  (opt, args) = parser.parse_args()

  theta = (opt.frame-1) * 2*math.pi / 199
  cc = c.rotate(Vector.AXIS_Y, theta)

  cmd.perform_render_density(parser, cc, density, color)

if __name__ == "__main__":
  main()
