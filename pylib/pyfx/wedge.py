"""
Utility code for generating wedges.
"""
import math

from operator import mul
from vr import vrend
from functools import reduce

class BaseWedge(object):
  PARAMETERS = ()

  def __init__(self):
    self._parameters = self.__parameters()
    self._max_bbox = None
  
  @property
  def parameters(self):
    return self._parameters

  @parameters.setter
  def parameters(self, value):
    self._parameters = value

  def compute_parameters(self, i):
    steps = self.compute_steps(i)
    compute = lambda u, l, t, i: u if t <= 1 else l + (u-l) * float(i) / (t-1)

    return tuple(compute(upper,lower,totalSteps,i) \
      for ((lower, upper, totalSteps), i) in zip(self.parameters, steps))

  @property
  def steps(self):
    return reduce(mul, self._steps(), 1)
 
  def get_bbox(self, i):
    raise NotImplementedError("Subclass must implement get_bbox")

  @property
  def max_bbox(self):
    if self._max_bbox != None: return self._max_bbox
    llc = vrend.Vector.MAX
    urc = vrend.Vector.MIN
    for i in range(self.steps):
      box = self.get_bbox(i)
      llc = llc.componentMin(box.llc())
      urc = urc.componentMax(box.urc())
    self._max_bbox = vrend.Box(llc,urc)
    return self._max_bbox

  def compute_steps(self, i):
    return self.__compute_steps(i, self._steps())

  def __compute_steps(self, i, steps):
    if len(steps) == 1: return (i,)
    return (i%steps[0],) + self.__compute_steps(i / steps[0], steps[1:])
    
  def _steps(self):
    return tuple((x[2] for x in self.parameters))

  def __parameters(self):
    return self.__class__.PARAMETERS
