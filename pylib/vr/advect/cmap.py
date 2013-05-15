"""
Characteristic map advection functions.
"""

from vr import vrend, common

def gridless(scheme, u, time, n, x=vrend.Identity):
  """
  Gridless advection.

  Given an advection scheme (a function in this module) advect the position
  field over the velocity field over a time interval for n steps.
  """
  for i in range(n):
    x = scheme(u, time / float(n), x)
  return x

def gridded(scheme, u, time, n, res, box,
    x = vrend.Identity,
    k=1,
    defaultValue=vrend.Vector.ZEROS):
  """
  Gridded advection.

  Given advection scheme, do in total n iterations of advection, stopping at
  every k-th iteration to sample to a grid.
  """
  count = 0

  while count < n:
    iterations = min(k, n-count)
    count += iterations
    this_timestep = time * float(iterations) / float(count)
    x = gridless(scheme, u, this_timestep, iterations, x=x)
    x = (vrend.Difference, x, vrend.Identity)
    x = common.asdensegrid(x, res, defaultValue, griddable=box)
    x = (vrend.Sum, x, vrend.Identity)

  return x

def sl(u, step, x=vrend.Identity):
  """
  Semi-Lagrangian advection.
  """
  return (vrend.Advect, x, (vrend.PassThrough, u, x), step)

def fb(u, step, x=vrend.Identity):
  """
  Forward-backward advection.
  """
  return sl(u, -step, sl(u, step, x))

def error(u, step, x=vrend.Identity):
  """
  Back and forth advection error function.
  """
  return (vrend.Amplify,
    (vrend.Difference, x, fb(u, step, x)),
    (vrend.Constant, 0.5))

def bfecc(u, step, x=vrend.Identity):
  """
  Back and forth error compensation and control advection.
  """
  return (vrend.Sum, sl(u, step, x), error(u, step, sl(u, step, x)))

def mm(u, step, x=vrend.Identity):
  """
  Modified MacCormack advection.
  """
  return (vrend.Sum, sl(u, step, x), error(u, step, x))
