import vrendtest
import math

from vr.vrend import *
from vr.common import *

"""
Default test case. For easy copying.
"""

class TestGinsu(vrendtest.VrendTestCase):

  def setUp(self):
    self.seq = Vector2DSequence()
    self.seq.push_back(Vector2D(0.25, 0.5))
    self.seq.push_back(Vector2D(0.50, 0.5))
    self.seq.push_back(Vector2D(0.75, 0.5))
    self.ginsu = (Ginsu, self.seq, Vector.AXIS_Z, Vector.AXIS_Y)
    self.ginsu = asvolumegraph(self.ginsu)


  def test_plane_march(self):
    p = Vector(0.25, 0.25, 0)
    step = Vector(0, 0, 1.25)

    last_value = self.ginsu.eval(p)
    p = p + step
    for i in range(20):
      current_value = self.ginsu.eval(p)
      self.assertEqual(last_value, current_value)
      p = p + step

  def test_up_march(self):
    p = Vector(0.25, 0.25, 10)
    step_size = 0.02
    step = Vector(0, step_size, 0)

    first_value = self.ginsu.eval(p)
    p = p + step
    total_step = step_size
    for i in range(20):
      self.assertClose(self.ginsu.eval(p), first_value - total_step)
      p = p + step
      total_step += step_size
  
  def test_right_march(self):
    p = Vector(0.25, 0.25, 10)
    step_size = 0.02
    step = Vector(step_size, 0, 0)

    last_value = self.ginsu.eval(p)
    p = p + step
    for i in range(20):
      current_value = self.ginsu.eval(p)
      self.assertClose(current_value, last_value)
      last_value = current_value
      p = p + step

