"""
This module is for inserting names into the vrend module that have since been
removed, or to facilitate ease of use.

Namely, this allows for convenient notation such as:

  from vrend import *
  (Sum, (Constant, 1), (Constant, 2))

When building volume functions. Notice that, though Sum and Constant are
templated classes we did not need to specify 'ScalarSum' or 'ScalarConstant'
which are the true generated classes from swig.
"""

import inspect

def __Type(thing):
  """
  Given a thing of discriminating type, return a string which guesses the
  prefix for a class which can handle it.
  """
  from vr import vrend, common
  if isinstance(thing, (int, float)): return 'Scalar'
  if isinstance(thing, vrend.Vector): return 'Vector'
  if isinstance(thing, vrend.Color): return 'Color'
  if isinstance(thing, vrend.Matrix): return 'Matrix'
  if common.isscalar(thing): return 'Scalar'
  if common.isvector(thing): return 'Vector'
  if common.ismatrix(thing): return 'Matrix'
  if common.iscolor(thing): return 'Color'
  raise ValueError("Could not guess type of: " + thing)

def __CallerName():
  return inspect.stack()[2][3]

def __Guess(field, *args):
  from vr import vrend
  guesses = []
  caller, type = __CallerName(), __Type(field)
  guesses.append(type + caller)
  guesses.append(caller + type)
  for guess in guesses:
    if hasattr(vrend, guess):
      return getattr(vrend, guess)(*args)
  raise ValueError("Could not guess a type for `%s`, not one of: %s" 
    % (caller, ", ".join(guesses)))

def Ramp(v1, v2, f1, f2): return __Guess(f1, v1, v2, f1, f2)
def Constant(c): return __Guess(c, c)
def Sum(f1, f2): return __Guess(f1, f1, f2)
def Difference(f1, f2): return __Guess(f1, f1, f2)
def Product(f1, f2): return __Guess(f1, f1, f2)
def Quotient(f1, f2): return __Guess(f1, f1, f2)
def Translate(direction, field): return __Guess(field, direction, field)
def Scale(factor, field): return __Guess(field, factor, field)
def Rotate(axis, theta, field): return __Guess(field, axis, theta, field)
def ReplaceBox(field, box): return __Guess(field, field, box)
def PassThrough(f1, v1): return __Guess(f1, f1, v1)
def Advect(f1, v1, dt): return __Guess(f1, f1, v1, dt)
def Amplify(f1, f2): return __Guess(f1, f1, f2)
def DenseGrid(g1, v1, c1): return __Guess(c1, g1, v1, c1)
def ReplaceGradient(f1, v1): return __Guess(f1, f1, v1)
