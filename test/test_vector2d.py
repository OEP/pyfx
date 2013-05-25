import vrendtest
import math

from pyfx import Vector2D, line_distance

class TestVector2D(vrendtest.VrendTestCase):

  def setUp(self):
    self.u = Vector2D(-0.25, 0)
    self.v = Vector2D( 0.25, 0)
    pass

  def test_linesegment_distance(self):
    step = Vector2D(0, 0.01)
    p = (self.u + self.v) / 2.0

    while p.y() < 1:
      dist = line_distance(p, self.u, self.v)
      self.assertClose(dist, p.y())
      p = p + step
