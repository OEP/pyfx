"""
Class for transforming densities.
"""
from vr import vrend

class Transform(object):

  def __init__(self,
      density = None,
      translate = vrend.Vector.ZEROS,
      rotate_axis = vrend.Vector.UZ,
      rotate_theta = 0.0,
      scale = 1.0):
    self.translate = translate
    self.rotate_axis = rotate_axis
    self.rotate_theta = rotate_theta
    self.scale = scale
    self.density = density

  @property
  def scale(self): return self._scale
  @scale.setter
  def scale(self, value): self._scale = value
  @property
  def translate(self): return self._translate
  @translate.setter
  def translate(self, value): self._translate = value

  @property
  def density(self):
    return self.transform(self._density)

  @density.setter
  def density(self, value):
    self._density = value

  def transform(self, out):
    if self.has_scale(): out = self.get_scale(out)
    if self.has_rotation(): out = self.get_rotation(out)
    if self.has_translation(): out = self.get_translation(out)
    return out

  def has_rotation(self): return self.rotate_theta != 0.0
  def has_translation(self): return self.translate != vrend.Vector.ZEROS
  def has_scale(self): return self.scale != 1.0

  def get_rotation(self, thing):
    return (vrend.Rotate, self.rotate_axis, self.rotate_theta, thing)

  def get_scale(self, thing):
    return (vrend.Scale, self.scale, thing)

  def get_translation(self, thing):
    return (vrend.Translate, self.translate, thing)

