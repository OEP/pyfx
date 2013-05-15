from vr import wedge, vrend, factory, cmd, common
from vr.graph import VolumeGraph

class BaseSplineGenerator(object):

  def __init__(self, cls, segments, defaults):
    self.cls = cls
    self.segments = segments
    self.defaults = defaults
    self._chain = None
    self._density = None
    self.attrs = frozenset(filter(lambda x: not x.startswith('_'), dir(cls)))
    self.__assert_valid_instance()

  @property
  def chain(self):
    if self._chain: return self._chain
    self._chain = []
    for i in range(self.segments):
      q = i / float(self.segments - 1)
      params = self.compute(q)
      self._chain.append(params)
    return self._chain

  def compute(self, q):
    "Compute anchor at normalized point q."
    params = self.cls()
    for attr in self.attrs:
      value = self.__compute_attr(attr, q)
      if value: setattr(params, attr, value)
    return params

  def __compute_attr(self, attr, q):
    """
    Computes a value for an attribute.

    The sequence is:
      1. Call the compute_<attr> method if it exists and use that value.
      2. Look up <attr> in given dictionary and use that value.
      3. Return None, signifying to use the default value.
    """
    if hasattr(self, 'compute_%s' % attr):
      tmp = getattr(self, 'compute_%s' % attr)
      return tmp(q)
    if attr in self.defaults:
      return self.defaults[attr]
    return None

  def __assert_valid_instance(self):
    computes = set((y.replace('compute_', '', 1) for y in 
      filter(lambda x: x.startswith('compute_'), dir(self))))
    defaultKeys = set(self.defaults.keys())
    self.__assert_valid_keys(computes, "Invalid compute_* method name(s): %s")
    self.__assert_valid_keys(defaultKeys, "Invalid default attribute name(s): %s")

  def __assert_valid_keys(self, keys, msgFmt="Invalid attribute name(s): %s"):
    invalid = set(keys) - self.attrs
    if len(invalid) > 0:
      tmp = ", ".join(invalid)
      raise ValueError(msgFmt % tmp)

class SplineFSPNGenerator(BaseSplineGenerator):
  """
  Generate FSPN parameters for spline-based effects.
  """
  def __init__(self, segments, defaults={}):
    super(SplineFSPNGenerator, self).__init__(vrend.Noise_t, segments, defaults)

class SplineWispGenerator(BaseSplineGenerator):

  def __init__(self, segments, seed, defaults={}, res=vrend.Vector(0.1,0.1,0.1), partitions=16):
    super(SplineWispGenerator, self).__init__(vrend.WispParameters, segments, defaults)
    self.res = res
    self.seed = seed
    self.partitions = partitions

  @property
  def density(self):
    if self._density: return self._density
    wchain = vrend.WispChain()
    for p in self.chain:
      wchain.push_back(p)
    self._density = VolumeGraph()
    self._density._track(wchain)
    wisp = vrend.SplineWisp(wchain, self.res, self.partitions)
    wisp.setSeed(self.seed)
    wisp.initialize()
    self._density.add(wisp)
    return self._density
