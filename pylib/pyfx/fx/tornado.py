import math

from vr import vrend, step
from vr.spline import SplineWispGenerator, SplineFSPNGenerator
from vr.interpolate import linear

class TornadoFSPN(SplineFSPNGenerator):
  "Generates fractal sum parameters for tornado."
  def __init__(self, segments, tornado, defaults={}):
    super(TornadoFSPN, self).__init__(segments, defaults=defaults)
    self.tornado = tornado

  def compute_translate(self, q):
    e2 = self.tornado.compute_E2(q)
    return (self.tornado.bottom - self.tornado.top) * q \
      - e2 * self.tornado.velocity * self.tornado.time

class Tornado(SplineWispGenerator):
  def __init__(self, segments, seed=0x7049AD0,
      fspn1_defaults = {},
      fspn2_defaults = {},
      defaults = {},
      top = vrend.Vector(0, 2, 0),
      bottom = vrend.Vector(0, 0, 0),
      delta = vrend.Vector(1, 0, 0),
      stepf_q0 = 0.5,
      stepf_L = 0.5,
      dots_top = int(5e6),
      pscale_top = 1.0,
      pscale_bottom = 0.3,
      pscale_rate = 1.0,
      velocity = 0.6,
      time = 0.0,
      frequency = 0.6, ## Approximately a 250-ft diameter @ 300 mph in Hz
      res = vrend.Vector(0.1,0.1,0.1),
      partitions = 16):
    super(Tornado, self).__init__(segments, seed,
      defaults=defaults,
      res=res,
      partitions=partitions)
    self.top = top
    self.bottom = bottom
    self.dots_top = dots_top
    self.pscale_top = pscale_top
    self.pscale_rate = pscale_rate
    self.pscale_bottom = pscale_bottom
    self.delta = delta
    self.stepf = (
      lambda q: step.tanh(q, stepf_q0, stepf_L),
      lambda q: step.dtanh(q, stepf_q0, stepf_L)
    )
    self.fspn1 = TornadoFSPN(segments, self, defaults=fspn1_defaults)
    self.fspn2 = TornadoFSPN(segments, self, defaults=fspn2_defaults)
    self.velocity = velocity
    self.time = time
    self.frequency = frequency

  def compute_FSPN1(self, q):
    return self.fspn1.compute(q)

  def compute_FSPN2(self, q):
    return self.fspn2.compute(q)

  def compute_E2(self, q):
    tmp = (self.bottom - self.top) + self.delta * self.stepf[1](q)
    tmp = tmp.unit()
    return tmp

  def compute_E1(self, q):
    axis = self.compute_E2(q)
    theta = self.time * self.frequency * 2 * math.pi
    bmt = self.bottom - self.top
    tmp = self.delta * bmt.magnitude() - \
          bmt * self.delta.magnitude() * self.stepf[1](q)
    tmp = tmp.rotate(axis, theta).unit()
    return tmp

  def compute_E0(self, q):
    (e2, e1) = (self.compute_E2(q), self.compute_E1(q))
    e0 = e1 ^ e2
    e0 = e0.unit()
    return e0

  def compute_P0(self, q):
    return linear(self.top, self.bottom, q) + self.delta * self.stepf[0](q)

  def compute_Dots(self, q):
    r = (self.compute_PScale(q) / self.pscale_top)
    r = r * r
    return int(self.dots_top * r)

  def compute_PScale(self, q):
    return max(
      math.exp(-q * self.pscale_rate) * self.pscale_top,
      self.pscale_bottom) 
