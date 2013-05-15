import vrendtest
import math

from vr.vrend import *

class TestVector2D(vrendtest.VrendTestCase):

  def setUp(self):
    self.u = Vector2D(-0.25, 0)
    self.v = Vector2D( 0.25, 0)
    pass

  def test_linesegment_distance(self):
    step = Vector2D(0, 0.01)
    p = (self.u + self.v) / 2.0

    while p.Y() < 1:
      dist = line_distance(p, self.u, self.v)
      self.assertClose(dist, p.Y())
      p = p + step
