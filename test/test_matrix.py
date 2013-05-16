import vrendtest
import math

from vr.vrend import Matrix, Vector

class TestMatrix(vrendtest.VrendTestCase):

  def setUp(self):
    self.a = Matrix(1,2,3,4,5,6,7,8,9)
    self.b = Matrix(1,1,1,1,1,1,1,1,1)
    self.c = Matrix(1,1,1,2,3,2,3,2,1)
    self.invc = Matrix(0.5, -0.5, 0.5, -2, 1, 0, 2.5, -0.5, -0.5)
    self.eye = Matrix.EYE

    self.ones = Vector(1, 1, 1)
    self.zeros = Vector(0, 0, 0)

  def test_add(self):
    self.assertEqual(self.a + self.b, Matrix(2,3,4,5,6,7,8,9,10))

  def test_subtract(self):
    self.assertEqual(self.a - self.b, Matrix(0,1,2,3,4,5,6,7,8))

  def test_dot(self):
    adb = Matrix(6,6,6,15,15,15,24,24,24)
    bda = Matrix(12,15,18,12,15,18,12,15,18)

    self.assertEqual(self.a * self.b, adb)
    self.assertEqual(self.b * self.a, bda)
    self.assertEqual(self.a * self.eye, self.a)
    self.assertEqual(self.eye * self.a, self.a)
    self.assertEqual(self.b * self.eye, self.b)
    self.assertEqual(self.eye * self.b, self.b)
    self.assertEqual(self.c * self.invc, self.eye)
    self.assertEqual(self.invc * self.c, self.eye)

  def test_determinant(self):
    self.assertEqual(self.c.determinant(), -2)

  def test_inverse(self):
    self.assertEqual(self.c.inverse(), self.invc)
