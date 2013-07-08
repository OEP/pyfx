"""

Implements most overloaded operator stuff.
"""
from .native import *

FIELDS = (ScalarField, VectorField, ColorField, MatrixField)
CONSTANTS = (int, float, Vector, Color, Matrix)

def atomize(obj, children=[], const=False):
  """
  Attempts to convert ``obj`` into a corresponding Atom.

  If children are given, the resulting Atom will adopt those children.
  Children will be ignored in cases where it does not make sense.

  If ``const`` is true, Python constants will be converted into their
  corresponding field types.
  """

  ## Fix constants if requested
  if const and isinstance(obj, CONSTANTS):
    return atomize(Constant(obj), const=const)

  ## Copy other atoms
  if isinstance(obj, Atom):
    return obj.copy()
  
  ## Treat lists/tuples as an operation call.
  if isinstance(obj, (list, tuple)):
    op = obj[0]
    args = obj[1:]
    children = [atomize(x, const=const) for x in args]
    top = op(*(x.top for x in children))
    return atomize(top, children=children, const=const)

  ## Handle the basic field types.
  if isinstance(obj, ScalarField):
    return ScalarFieldAtom(obj, children)
  if isinstance(obj, VectorField):
    return VectorFieldAtom(obj, children)
  if isinstance(obj, ColorField):
    return ColorFieldAtom(obj, children)
  if isinstance(obj, MatrixField):
    return MatrixFieldAtom(obj, children)

  ## If it's a callable just call it with no-args
  if hasattr(obj, '__call__'):
    return atomize(obj())
  
  ## Otherwise, just wrap as is and pray the user knows what he's doing.
  return Atom(obj, children)

class Atom(object):
  def __init__(self, top, children=[]):
    self.top = top
    self.children = list(children)
  
  def __getattr__(self, name):
    return getattr(self.top, name)

  def copy(self):
    cls = self.__class__
    return cls(self.top, list(self.children))
  
  def adopt(self, obj):
    self.children.append(obj)

class BaseFieldAtom(Atom):
  def __add__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Sum, self, o))
    return NotImplemented
  
  def __radd__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Sum, o, self))
    return NotImplemented

  def __sub__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Difference, self, o))
    return NotImplemented

  def __rsub__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Difference, o, self))
    return NotImplemented
 
  def __mul__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Product, self, o))
    return NotImplemented

  def __rmul__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Product, o, self))
    return NotImplemented

  def __truediv__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Quotient, self, o))
    return NotImplemented

  def __rtruediv__(self, o):
    o = atomize(o, const=True)
    if type(self) == type(o):
      return atomize((Quotient, o, self))
    return NotImplemented

  def __abs__(self):
    return atomize((AbsoluteValue, self))

  def translate(self, v):
    return atomize((Translate, v, self))

  def replace_box(self, box):
    return atomize((ReplaceBox, self, box))

  def rotate(self, axis, theta):
    return atomize((Rotate, axis, theta, self))

  def scale(self, amt):
    return atomize((Scale, amt, self))
  
  def pass_through(self, x):
    return atomize((PassThrough, self, x), const=True)

class ScalarFieldAtom(BaseFieldAtom):
  def union(self, o, *args):
    o = atomize((Union, self, o))
    for x in args:
      o = atomize((Union, o, x))
    return o

  def cutout(self, o, *args):
    o = atomize((Cutout, self, o))
    for x in args:
      o = atomize((Cutout, o, x))
    return o

  def intersect(self, o, *args):
    o = atomize((Intersect, self, o))
    for x in args:
      o = atomize((Intersect, o, x))
    return o

  def blinn(self, o, *args):
    o = atomize((Blinn, self, o))
    for x in args:
      o = atomize((Blinn, x, o))
    return o

  def mask(self):
    return atomize((Mask, self))

  def clamp(self, lo=0, hi=1, q=1.0):
    return atomize((Clamp, self, lo, hi, q))

  def shade(self, color):
    color = atomize(color, const=True) 
    return atomize((Amplify, color, self.mask()))

  def find_surface(self):
    return atomize((FindSurface, self))

  def advect(self, u, dt):
    return atomize((Advect, self, u, dt)) 

  def gradient(self):
    return atomize((Gradient, self))

  def __pow__(self, o):
    return atomize((Power, self, o), const=True)

  def __rpow__(self, o):
    return atomize((Power, o, self), const=True)

class VectorFieldAtom(BaseFieldAtom):
  def cross(self, o):
    return atomize((CrossProduct, self, o), const=True)

  def dot(self, o):
    return atomize((DotProduct, self, o), const=True)

  def outer_product(self, o):
    return atomize((OuterProduct, self, o), const=True)
  
  def __mul__(self, o):
    '''Vector times scalar'''
    o = atomize(o, const=True)
    if isinstance(o, ScalarFieldAtom):
      return atomize((Amplify, self, o))
    return super(VectorFieldAtom, self).__mul__(self, o)

  def __rmul__(self, o):
    '''Scalar times vector'''
    return self * o

  def __truediv__(self, o):
    '''Vector by scalar'''
    x = o
    if isinstance(o, (int, float)):
      x = 1.0 / x
      return self * x
    x = atomize(x, const=True)
    if isinstance(o, ScalarFieldAtom):
      return atomize((Damp, self, o))
    return super(VectorFieldAtom, self).__truediv__(self, o)
    
class ColorFieldAtom(BaseFieldAtom):
  def merge(self, first, *args, mode=MergeColor.MAX):
    o = atomize((MergeColor, self, first, mode))
    for x in args:
      o = atomize((MergeColor, o, x, mode))
    return o
  
  def __mul__(self, o):
    '''Color times scalar'''
    o = atomize(o, const=True)
    if isinstance(o, ScalarFieldAtom):
      return atomize((Amplify, self, o))
    return super(VectorFieldAtom, self).__mul__(self, o)

  def __rmul__(self, o):
    '''Scalar times color'''
    return self * o

class MatrixFieldAtom(BaseFieldAtom):
  pass
