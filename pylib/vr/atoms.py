"""

Implements most overloaded operator stuff.
"""
from vr.vrend import *

FIELDS = (ScalarField, VectorField, ColorField, MatrixField)
CONSTANTS = (int, float, Vector, Color, Matrix)

def as_field(obj):
  if isinstance(obj, Atom):
    return obj

  if isinstance(obj, FIELDS):
    return obj
  
  if isinstance(obj, CONSTANTS):
    return Constant(obj)

  raise TypeError("Can't convert to field", obj)

def atomize(obj, children=[]):
  ## Copy other atoms
  if isinstance(obj, Atom):
    return obj.copy()
  
  ## Treat lists/tuples as an operation call.
  if isinstance(obj, (list, tuple)):
    op = obj[0]
    args = obj[1:]
    children = map(atomize, args)
    top = op(*(x.top for x in children))
    return atomize(top, children)

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
    return atomize(obj(), children)
  
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
  
  def acquire(self, obj):
    self.children.append(obj)

class BaseFieldAtom(Atom):
  def __add__(self, o):
    return atomize((Sum, self, as_field(o)))
  
  def __radd__(self, o):
    return atomize((Sum, as_field(o), self))

  def __sub__(self, o):
    return atomize((Difference, self, as_field(o)))

  def __rsub__(self, o):
    return atomize((Difference, as_field(o), self))
 
  def __mul__(self, o):
    return atomize((Product, self, as_field(o)))

  def __rmul__(self, o):
    return atomize((Product, as_field(o), self))

  def __div__(self, o):
    return atomize((Quotient, self, as_field(o)))

  def __rdiv__(self, o):
    return atomize((Quotient, as_field(o), self))

  def translate(self, v):
    return atomize((Translate, v, self))

  def rotate(self, axis, theta):
    return atomize((Rotate, axis, theta, self))

  def scale(self, amt):
    return atomize((Scale, amt, self))

class ScalarFieldAtom(BaseFieldAtom):
  def mask(self):
    return atomize((Mask, self))

  def clamp(self, lo=0, hi=1, q=1.0):
    return atomize((Clamp, lo, hi, q))

  def shade(self, color):
    color = atomize(as_field(color)) 
    return atomize((Amplify, color, self.mask()))

class VectorFieldAtom(BaseFieldAtom):
  def cross(self, o):
    return atomize((CrossProduct, self, o))

  def dot(self, o):
    return atomize((DotProduct, self, o))

  def outerProduct(self, o):
    return atomize((OuterProduct, self, o))

class ColorFieldAtom(BaseFieldAtom):
  pass

class MatrixFieldAtom(BaseFieldAtom):
  pass
