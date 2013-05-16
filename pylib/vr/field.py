"""
Field wrapper class.

Implements most overloaded operator stuff.
"""
from vr.vrend import *

class Field(object):
  
  def __init__(self, obj):
    self.children = []

    ## Wrap constants with Constant field
    if isinstance(obj, (int, float, Vector, Matrix)):
      self.top = Constant(obj)
      return

    ## Copy other fields
    if isinstance(obj, Field):
      self.top = obj.top
      self.children = list(obj.children)
      return
    
    ## Treat lists/tuples as an operation call.
    if isinstance(obj, (list, tuple)):
      op = obj[0]
      args = obj[1:]
      args = map(Field, args)
      self.children.extend(args)
      self.top = op(*(x.top for x in args))
      return

    ## If it's a callable just call it with no-args
    if hasattr(obj, '__call__'):
      self.top = obj()
      return
    
    ## Otherwise, just wrap as is and pray the user knows what he's doing.
    self.top = obj

  def __getattr__(self, name):
    return getattr(self.top, name)

  def __add__(self, o):
    return Field((Sum, self, o))
  
  def __radd__(self, o):
    return Field((Sum, o, self))

  def __sub__(self, o):
    return Field((Difference, self, o))

  def __rsub__(self, o):
    return Field((Difference, o, self))
 
  def __mul__(self, o):
    return Field((Product, self, o))

  def __rmul__(self, o):
    return Field((Product, o, self))

  def __div__(self, o):
    return Field((Quotient, self, o))

  def __rdiv__(self, o):
    return Field((Quotient, o, self))

  def acquire(self, obj):
    self.children.append(obj)
