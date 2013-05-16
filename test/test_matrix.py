import vrendtest
import math

from vr.vrend import Matrix, Vector

class TestMatrix(vrendtest.VrendTestCase):

  def setUp(self):
    self.a = Matrix(1,2,3,4,5,6,7,8,9)
    self.b = Matrix(1,1,1,1,1,1,1,1,1)
    self.eye = Matrix.EYE

    self.ones = Vector(1, 1, 1)
    self.zeros = Vector(0, 0, 0)

  def test_add(self):
    self.assertEqual(self.a + self.b, Matrix(2,3,4,5,6,7,8,9,10))

  def test_subtract(self):
    self.assertEqual(self.a - self.b, Matrix(0,1,2,3,4,5,6,7,8))

  def test_dot(self):
    pass

  def test_determinant(self):
    pass
