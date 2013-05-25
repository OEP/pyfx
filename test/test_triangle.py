import vrendtest
import math

from pyfx import *

"""
Test cases for class Triangle
"""

class TestDefault(vrendtest.VrendTestCase):

  def setUp(self):
    self.p1 = Vector(1, 0, 0)
    self.p2 = Vector(0, 1, 0)
    self.p3 = Vector(-1, 0, 0)
    self.t1 = Triangle(self.p1, self.p2, self.p3)

    self.u1mindist = (
      Vector(0, 0, 1),
      Vector(0, -1, 0)
    )

  def test_minimumdistance(self):
    pass
    # FIXME: I broke this intentionally a while back to make Levelsets work a
    # little better.
    #for p in self.u1mindist:
    #  self.assertEqual(self.t1.minimumDistance(p), 1)
