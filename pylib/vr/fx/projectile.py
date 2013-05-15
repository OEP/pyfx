'''
Class for a volume that travels like a projectile
'''
from vr import vrend

class Projectile(object):

  def __init__(self,
      density = None,
      initial_velocity = vrend.Vector.ZEROS,
      g = vrend.Vector(0.0,-0.1,0.0),
      time = 0.0):
    self.density = density
    self.initial_velocity = initial_velocity
    self.g = g
    self.time = time

  @property
  def time(self): return self._time
  @time.setter
  def time(self,value): self._time = value

  @property
  def density(self):
    return self.translate(self._density)

  @density.setter
  def density(self, value):
    self._density = value

  def translate(self, thing):
    return (vrend.Translate, self.initial_velocity * self.time + self.g * self.time * self.time, thing)
