"""
Class for building particles.
"""

from vr.fx.transform import Transform
from vr import vrend

class Particle(Transform):
  def __init__(self,
      time=0.0, 
      angular_axis = vrend.Vector.AXIS_Z,
      angular_velocity = 0.0,
      **kwargs):
    self.angular_axis = angular_axis
    self.angular_velocity = angular_velocity
    self.time = time
    super(Particle, self).__init__(**kwargs)

  ## Alias for particle lingo-matching.
  position = Transform.translate

  @property
  def angular_rotation(self):
    return self.angular_velocity * self.time

  def has_dynamic_rotation(self):
    return self.time * self.angular_velocity != 0.0

  def has_rotation(self):
    super_has_rotation = super(Particle, self).has_rotation
    return super_has_rotation() or self.has_dynamic_rotation()

  def get_rotation(self, thing):
    super_rotation = super(Particle, self).get_rotation(thing)
    if not self.has_dynamic_rotation(): return super_rotation

    return (vrend.Rotate, self.angular_axis, self.angular_rotation,
      super_rotation)
