from optparse import OptionParser

import math
import sys
import time
from datetime import date

import vr.vrend as vrend
import vr.render as render
import vr.path as path
from vr.vob import VOB

def getopt_levelset(parser=None, default_name=None):
  """
  Get parameters dealing with levelset generation.
  """
  if parser == None: parser = getopt_paths(default_name=default_name)
  parser = getopt_grid(parser=parser)
  parser.add_option("-t", "--threshold", dest='threshold', type=float,
    default=0.5, help="Theshold for levelset algorithm")
  parser.add_option("-p", "--padding", dest='padding', type=float,
    nargs=3, default=(0.3, 0.3, 0.3), help="Padding amount for bounding box")
  return parser

def getopt_grid(parser=None):
  """
  Parameters dealing with grids.
  """
  if parser == None: parser = OptionParser()
  parser.add_option("-G", "--grid-resolution", dest='grid_resolution', type=float,
    nargs=3,default=(0.1,0.1,0.1), help='Grid resolution')
  return parser

def getopt_paths(parser=None, default_name=None):
  """
  Get parameters dealing with paths.
  """
  if default_name == None: default_name = "out"
  if parser == None: parser = OptionParser()
  parser.add_option("-o", "--output", dest='output',
    default=".", help="Output directory")
  parser.add_option("-n", "--name", dest='name',
    default=default_name, help="Name of output file")
  parser.add_option("-f", "--frame", dest='frame', type=int,
    default=1, help="Frame number")
  return parser

def getopt_render(parser=None, default_name=None):
  """
  Get parameters dealing with volume rendering.
  """
  parser = getopt_paths(parser=parser, default_name=default_name)

  parser.add_option("-q", "--quality", dest='quality', type=str,
    default='quick', help='Render quality (quick, smallbeauty, beauty)')
  parser.add_option("-m", "--mode", dest='mode', type=str,
    default='render', help='Render mode (render, estimate)')

  ## Render option overrides
  parser.add_option("-s", "--step", dest='step', type=float,
    default=None, help='Ray step amount')
  parser.add_option("-r", "--rays", dest='rays', type=int,
    default=None, help='Rays per pixel')
  parser.add_option("-z", "--resolution", dest='resolution', type=int,
    default=None, help='Image resolution')
  parser.add_option("-k", "--scatter", dest='scatter', type=float,
    default=1.0, help='Scattering coefficient (kappa)')
  parser.add_option("-D", "--dsm-size", dest='dsm_size', type=int,
    default=256, help='Deep shadow map grid size')
  parser.add_option("--dsm-step", dest='dsm_step', type=float,
    default=0.01, help='Deep shadow map step size')
  parser.add_option("-S", "--dsm-samples", dest='dsm_samples', type=int,
    default=1, help='Number of raymarches per voxel in DSM')

  parser.add_option("-e", "--eye", dest='eye', type=float, nargs=3,
    default=(0, 0, 0), help='Camera position')
  parser.add_option("-v", "--view", dest='view', type=float, nargs=3,
    default=(0, 0, -1), help='Camera view vector; default looks at volume')
  parser.add_option("-u", "--up", dest="up", type=float, nargs=3,
    default=(0, 1, 0), help='Camera up vector; default is arbitrary')
  parser.add_option("-l", "--light", dest="lights", type=float, nargs=4,
    action='append', default=[],
    help='Add white light to scene (can be used multiple times)')
  parser.add_option("--fov", dest='fov', type=float, default=None,
    help='Override the FOV for the lights.')

  return parser

def getopt_transform(parser=OptionParser()):
  parser.add_option("-T", "--translate", dest="translate", type=float, nargs=3,
    default=None, help='Subject translation')
  parser.add_option("-R", "--rotate", dest="rotate", type=float, nargs=4,
    default=None, help='Subject rotation; expects axis and theta (in degrees)')
  parser.add_option("-L", "--scale", dest="scale", type=float, nargs=1,
    default=None, help='Subject scale')
  return parser

def fetch_quality(opt):
  return render.get_quality(opt.quality, (opt.resolution, opt.rays, opt.step))

def fetch_step(opt):
  mode = fetch_quality(opt)
  return mode[2]

def fetch_eye(opt):
  """Fetch eye vector from arguments."""
  return vrend.Vector(*opt.eye)

def fetch_view(opt):
  """Fetch view vector from arguments."""
  return vrend.Vector(*opt.view)

def fetch_up(opt):
  """Fetch up vector."""
  return vrend.Vector(*opt.up)

def fetch_rotation(opt):
  """Fetch rotation parameters from arguments"""
  if opt.rotate == None: return None
  axis = opt.rotate[:3]
  theta = math.radians(opt.rotate[3])
  return (vrend.Vector(*axis), theta)

def fetch_camera(opt):
  eye = fetch_eye(opt)
  view = fetch_view(opt)
  up = fetch_up(opt)
  cam = vrend.Camera()
  cam.setEyeViewUp(eye, view, up)
  return cam

def fetch_scene(opt, density, color=None, camera=None):
  camera = camera or fetch_camera(opt)
  scene = vrend.Scene(camera, density, color, opt.scatter, opt.dsm_size)
  scene.setDSMSamples(opt.dsm_samples)
  return scene

def fetch_translate(opt):
  if opt.translate == None: return None
  return vrend.Vector(*opt.translate)

def transform_subject(opt, subject):
  import vr.tree as tree
  rotateParms = fetch_rotation(opt)
  translate = fetch_translate(opt)
  if opt.scale:
    subject = tree.scaleAll(opt.scale, subject)
  if rotateParms: 
    subject = tree.rotateAll(rotateParms[0], rotateParms[1], subject)
  if translate:
    subject = tree.translateAll(translate, subject)
  return atomize(subject)

def compute_step(opt):
  quality = _compute_quality(opt)
  return quality[2]

def perform_get_levelset(parser, model):
  (opt, args) = parser.parse_args()
  res = vrend.Vector(*opt.grid_resolution)
  pad = vrend.Vector(*opt.padding)

  print "Resolution: ", common.strvector(res)
  print "Padding: ", common.strvector(pad)
  print "Threshold: ", opt.threshold

  with timer.print_on_finish("Levelset Generation"):
    levelset = vrend.Levelset(model, res, pad, opt.threshold)

  return levelset

def perform_write_levelset(parser, model):
  (opt, args) = parser.parse_args()
  levelset = perform_get_levelset(parser, model)
  vob = VOB.fromgrid(levelset.getGrid())

  vob.setprop('info-threshold', opt.threshold)
  vob.setprop('info-padding', opt.padding)

  perform_write_grid(parser, vob)

def perform_write_grid(parser, vob):
  (opt, args) = parser.parse_args()
  vobpath = path.grid_frame(opt.name, opt.frame, opt.output)

  with timer.print_on_finish("Write Grid"):
    vob.write(vobpath)

  print "Wrote grid to %s" % vobpath

def fetch_renderer(opt):
  renderer = render.Render(
    quality = render.get_quality(opt.quality, fetch_quality(opt)),
    scatter = opt.scatter,
    dsm_step = opt.dsm_step,
    dsm_size = opt.dsm_size,
    name = opt.name,
    frame = opt.frame,
    light_fov = opt.fov,
    output = opt.output
  )

  for (x, y, z, intensity) in opt.lights:
    light = vrend.Light(vrend.Vector(x,y,z), vrend.Color(1), intensity)
    renderer.add_light(light)
  return renderer
