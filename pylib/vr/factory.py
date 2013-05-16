"""
Functions for more easily generating complex objects.

This module provides easier interfaces for creating objects with complex
signatures.
"""

import vrend
import common
import interpolate

class BaseFactory(dict):
  CLASS = object
  FIELDS = ()

  def __init__(self, *args, **kwargs):
    self._assert_keys_valid(kwargs.keys())
    values = dict(tuple(self._values_as_dict().items())
      + tuple(kwargs.items()))
    dict.__init__(self, *args, **values)
    self._arglist = None
    self._instance = None
    self.interps = self._interps_as_dict()

  def create(self):
    if self._instance: return self._instance
    self._instance = self.__class__.CLASS(*self.arglist)
    return self._instance

  def __setitem__(self, key, value):
    self._arglist = None
    self._instance = None
    self._assert_key_valid(key)
    dict.__setitem__(self, key, value)

  @property
  def arglist(self):
    if self._arglist: return self._arglist
    self._arglist = ()
    for key in self._field_names():
      arg = self[key]
      if isinstance(arg, BaseFactory): arg = arg.create()
      self._arglist += (arg,)
    return self._arglist

  def interpolate(self, other, q):
    cls = self.__class__
    out = {}
    for (key, fn) in self.interps.items():
      a = self[key]
      b = other[key]
      out[key] = fn(a, b, q)
    return cls(**out)

  def _field_count(self):
    return len(self.__defaults())

  def __defaults(self):
    return self.__class__.FIELDS

  def _values_as_dict(self):
    return dict(zip(self._field_names(), self._field_values()))

  def _interps_as_dict(self):
    return dict(zip(self._field_names(), self._field_interps()))

  def _field_names(self):
    return (x[0] for x in self.__defaults())

  def _field_values(self):
    for x in self.__defaults():
      try:
        yield x[1]()
      except TypeError:
        yield x[1]

  def _field_interps(self):
    return (x[2] for x in self.__defaults())
  
  def _assert_key_valid(self, key):
    if key not in self._field_names():
      raise ValueError("Unexpected field name: %s" % key)

  def _assert_keys_valid(self, keys):
    for key in keys:
      self._assert_key_valid(key)

class FractalSumFactory(BaseFactory):
  CLASS = vrend.FractalSum
  FIELDS = (
    ('noise', vrend.PerlinNoise, interpolate.left),
    ('octaves', 3.0, interpolate.linear),
    ('fjump', 2.0, interpolate.linear),
    ('roughness', 0.5, interpolate.linear),
    ('frequency', 0.666666, interpolate.linear),
    ('translate', vrend.Vector(0), interpolate.linear),
    ('offset', 0.0, interpolate.linear),
    ('axis', vrend.Vector.UZ, interpolate.rotate),
    ('angle', 0.0, interpolate.linear),
  )

class WispFactory(BaseFactory):
  CLASS = vrend.Wisp
  FIELDS = (
    ('fspn1', FractalSumFactory, interpolate.factory),
    ('fspn2', FractalSumFactory, interpolate.factory),
    ('e0', vrend.Vector.UX, interpolate.rotate),
    ('e1', vrend.Vector.UY, interpolate.rotate),
    ('e2', vrend.Vector.UZ, interpolate.rotate),
    ('p0', vrend.Vector(0), interpolate.linear),
    ('scale', vrend.Vector.ONES, interpolate.linear),
    ('delta', vrend.Vector(0), interpolate.linear),
    ('pscale', 1.0, interpolate.linear),
    ('clump', 0.333, interpolate.linear),
    ('amp', 1.0, interpolate.linear),
    ('wispDots', int(5e6), interpolate.left),
    ('opacity', 1.0, interpolate.linear),
    ('res', vrend.Vector(0.1, 0.1, 0.1), interpolate.linear),
    ('defaultValue', 0, interpolate.left),
    ('partitionSize', 16, interpolate.left),
    ('box', None, interpolate.left),
  )

  def get_bbox(self):
    """Calculates bounding box without allocating Wisp"""
    if self['box']: return self['box']
    fspn1 = self.arglist[0]
    fspn2 = self.arglist[1]
    return vrend.Wisp.calcBBox(
      fspn1,
      fspn2,
      self['clump'],
      self['scale'],
      self['p0'])

class PyroclasticSphereFactory(BaseFactory):
  CLASS = vrend.PyroclasticSphere
  FIELDS = (
    ('fs', FractalSumFactory, interpolate.factory),
    ('radius', 1.0, interpolate.linear),
    ('gamma', 1.0, interpolate.linear),
    ('amp', 1.0, interpolate.linear),
  )

class StampedNoiseFactory(BaseFactory):
  CLASS = vrend.StampedNoise
  FADE = common.asvolumegraph((vrend.Clamp, vrend.Sphere, 0, 1))
  FIELDS = (
    ('noise', FractalSumFactory, interpolate.factory),
    ('fade', FADE.top(), interpolate.left),
    ('gamma', 1.0, interpolate.linear),
  )
