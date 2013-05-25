"""
Various interpolation styles.
"""
import math

def left(a, b, q):
  """Identity -- return left argument."""
  return a

def right(a, b, q):
  """Identity -- return right argument."""
  return b

def linear(a, b, q):
  """Basic linear interpolation."""
  return a * (1-q) + b * q

def rotate(a, b, q):
  """Linear rotation about an axis between vectors."""
  bxa = b ^ a
  au = a.unit()
  m1 = a.magnitude()
  m2 = b.magnitude()

  if bxa.magnitude() == 0:
    return au * linear(m1, m2, q)

  axis = bxa.unit()
  theta = math.acos((a * b) / (m1 * m2))
  return au.rotate(axis, linear(0.0, theta, q)) * linear(m1, m2, q)

def factory(a, b, q):
  """Factory interpolation."""
  return a.interpolate(b, q)
