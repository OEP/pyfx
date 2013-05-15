import vrendtest
import math

from vr.vrend import *

"""
Default test case. For easy copying.
"""

class TestDenseGrid(vrendtest.VrendTestCase):

  def setUp(self):
    self.b1 = Box(Vector(0,0,0), Vector(1,1,1))
    self.r1 = Vector(0.1,0.1,0.1)
    self.r2 = Vector(0.09,0.09,0.09)

    self.d1 = ScalarDenseGrid(self.b1, self.r1, 0)
    self.d2 = ScalarDenseGrid(self.b1, self.r2, 0)
    self.d3 = ScalarDenseGrid(self.b1, self.r2, 0)

    for k in range(self.d3.NZ()):
      for j in range(self.d3.NY()):
        for i in range(self.d3.NX()):
          if i % 2 == 0 and j % 2 == 0 and k % 2 == 0:
            self.d3.set(i,j,k,1)
          elif i % 2 == 1 and j % 2 == 1 and k % 2 == 1:
            self.d3.set(i,j,k,1)

  def test_eval(self):
    for i in range(self.d1.NX()):
      i = i + 0.5
      v = Vector(i,0,0)
      value = self.d3.eval(v)

  def test_gridsize(self):
    self.assertEqual(self.d1.size(), 10*10*10)
    self.assertEqual(self.d2.size(), 12*12*12)

  def test_setget(self):
    for k in range(self.d1.NZ()):
      for j in range(self.d1.NY()):
        for i in range(self.d1.NX()):
          self.assertEqual(self.d1.evalGrid(i,j,k), 0)
          self.d1.set(i,j,k,1.0)
          self.assertEqual(self.d1.evalGrid(i,j,k), 1)
