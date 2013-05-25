"""
Direct advection functions.

Direct advection is the advection of the density directly, not space itself.
"""

from vr import vrend, common
from vr.advect import cmap

def gridless(scheme, density, u, time, n):
  """
  Gridless advection.

  Given an advection scheme (a function in this module) advect the density
  over the velocity field over a time interval for n steps.
  """
  out = density
  for i in range(n):
    out = scheme(out, u, time / float(n))
  return out

def sl(density, u, step, x=vrend.Identity):
  """
  Semi-Lagrangian advection.
  """
  return (vrend.PassThrough, density, cmap.sl(u, step, x))

def fb(density, u, step, x=vrend.Identity):
  """
  Forward-backward advection.
  """
  return (vrend.PassThrough, density, cmap.fb(u, step, x))

def error(density, u, step, x=vrend.Identity):
  """
  Back and forth advection error function.
  """
  return (vrend.Product,
    (vrend.Constant, 0.5),
    (vrend.Difference,
      (vrend.PassThrough, density, x),
      fb(density, u, step, x)))

def bfecc(density, u, step, x=vrend.Identity):
  """
  Back and forth error compensation and control advection.
  """
  return (vrend.Sum, sl(density, u, step, x),
    error(density, u, step, cmap.sl(u, step, x)))

def mm(density, u, step, x=vrend.Identity):
  """
  Modified MacCormack advection.
  """
  return (vrend.Sum, sl(density, u, step, x), error(density, u, step, x))
