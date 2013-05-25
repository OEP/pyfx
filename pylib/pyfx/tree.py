"""
Utility functions for manipulating volume trees.
"""

import vrend as vr
from inspect import isclass

def _chainBinOp(op, things):
  if len(things) == 0: return things
  if len(things) == 1: return things[0]
  return (op, things[0], _chainBinOp(op, things[1:]))

def shell(h1, f1):
  return (vr.Cutout, (vr.ScalarSum, f1, (vr.ScalarConstant, h1)), f1)

def clampDensity(tree, r0, e0=0, e1=1):
  return (vr.Clamp, (vr.ScalarQuotient, tree, (vr.ScalarConstant, r0)), e0, e1)

def cutoutAll(*args):
  if len(args) == 1: return args[0]
  if len(args) == 2: return (vr.Cutout, args[0], args[1])
  result = (vr.Cutout, args[0], args[1])
  remaining = [result] + list(args[2:])
  return cutoutAll(*remaining)

def intersectAll(*args):
  return _chainBinOp(vr.Intersect, args)

def unionAll(*args):
  return _chainBinOp(vr.Union, args)

def multiplyAll(*args):
  return _chainBinOp(vr.ScalarProduct, args)

def blinnAll(c, *args):
  return (vr.Blinn, args[0], multiplyAll(*args[1:]), c)

def scaleAll(factor, *args):
  if len(args) == 1: return (vr.Scale, factor, args[0])
  return tuple((vr.Scale, factor, x) for x in args)

def rotateAll(axis, theta, *args):
  if len(args) == 1: return (vr.Rotate, axis, theta, args[0])
  return tuple((vr.Rotate, axis, theta, x) for x in args)

def translateAll(direction, *args):
  if len(args) == 1: return (vr.Translate, direction, args[0])
  return tuple((vr.Translate, direction, x) for x in args)

def mergeAll(mode, *args):
  if len(args) == 0: return args
  if len(args) == 1: return args[0]
  return (vr.MergeColor, args[0], mergeAll(mode, *args[1:]), mode)

def constantProduct(tree, k):
  if issubclass(tree[0], vr.ScalarField) and isinstance(k, (int,float,long)):
    return (vr.ScalarProduct, tree, (vr.ScalarConstant, k))
  raise ValueError("Can't multiply %s and %s!" % (tree, k))

def __scalar2rgba(tree, color, fn):
  colorfield = None
  if isinstance(color, vr.Color):
    colorfield = (vr.ColorConstant, color)
  elif isinstance(color, tuple) and \
    isclass(color[0]) and \
    issubclass(color[0], vr.ColorField):
    colorfield = color
  else:
    color = vr.Color(color[0], color[1], color[2], color[3])
    colorfield = (vr.ColorConstant, color)
  return (vr.AmplifyColor, colorfield, fn(tree))
  

def scalar2rgba(tree, color=(1,1,1,1), clampAmt=1):
  """Converts a scalar field into a color field for quick visualization."""
  return __scalar2rgba(tree, color,
    lambda x: (vr.Clamp, x, 0, clampAmt)
  )

def shadeScalar(tree, color):
  return __scalar2rgba(tree, color,
    lambda x: (vr.Mask, x)
  )

def vector2rgba(tree, mask):
  """Given a vector field, visualize as a Color field."""
  return (vr.CreateColor,
    (vr.Clamp, (vr.VectorComponent, tree, vr.Vector.COMPONENT_X), 0, 1),
    (vr.Clamp, (vr.VectorComponent, tree, vr.Vector.COMPONENT_Y), 0, 1),
    (vr.Clamp, (vr.VectorComponent, tree, vr.Vector.COMPONENT_Z), 0, 1),
    (vr.Clamp, mask, 0.0, 1.0))

def grad2rgba(tree):
  """Given a volume tree, visualize its gradient as a Color field."""
  return vector2rgba((vr.Gradient, tree), tree)
