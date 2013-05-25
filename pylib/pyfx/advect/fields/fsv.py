from vr import common, vrend
from vr.factory import FractalSumFactory

class FractalSumVelocity(object):
  """
  Generates a cross product of two fractal sum velocity fields.
  """

  def __init__(self,
      fsf1 = FractalSumFactory(),
      fsf2 = FractalSumFactory(),
      d1 = vrend.Vector(1,1,1),
      d2 = vrend.Vector(-1,-1,-1),
      d3 = vrend.Vector(2,2,2),
      d4 = vrend.Vector(-2,-2,-2)):
    self.fsf1 = fsf1
    self.fsf2 = fsf2
    self.d1 = d1
    self.d2 = d2
    self.d3 = d3
    self.d4 = d4

  @property
  def field(self):
    fs1 = self.fsf1.create()
    fs2 = self.fsf2.create()

    field1 = atomize((vrend.VectorFractalSumField, fs1, self.d1, self.d2))
    field2 = (vrend.VectorFractalSumField, fs2, self.d3, self.d4)
    return field1.cross(field2)

FIELD1 = FractalSumVelocity(
  fsf1 = FractalSumFactory(
    fjump = 2,
    frequency = 4,
    roughness = 0.85,
    octaves = 8.5
  ),
  fsf2 = FractalSumFactory(
    fjump = 1.2,
    roughness = 0.75,
    frequency = 6,
    octaves = 8.5
  )
)
