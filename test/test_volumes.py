import vrendtest
import math

from pyfx import atomize, Vector

class TestVectorVolumes(vrendtest.VrendTestCase):

  def setUp(self):
    self.v1 = atomize(Vector(1, 0, 0), const=True)
    self.v2 = atomize(Vector(0, 1, 0), const=True)
    self.v3 = atomize(Vector(0, 0, 1), const=True)

    self.f0 = atomize(0, const=True)
    self.f1 = atomize(1, const=True)
    self.f2 = atomize(2, const=True)
    self.f3 = atomize(3, const=True)

    self.t1 = Vector(0,0,0)
    self.t2 = Vector(1,1,1)

  def test_scalar_product(self):
    p1 = self.f0 * self.f1
    p2 = self.f1 * self.f0
    p3 = self.f2 * self.f3
    self.assertEqual(p1.eval(self.t1), 0)
    self.assertEqual(p2.eval(self.t1), 0)
    self.assertEqual(p3.eval(self.t1), 6)
    self.assertVectorClose(p1.grad(self.t1), Vector(0,0,0))
    self.assertVectorClose(p2.grad(self.t1), Vector(0,0,0))
    self.assertVectorClose(p3.grad(self.t1), Vector(0,0,0))

  def test_vector_sum(self):
    sum1 = (self.v1 + self.v2)
    sum2 = (self.v2 + self.v3)
    sum3 = (self.v1 + self.v3)
    self.assertVectorClose(sum1.eval(self.t1), Vector(1,1,0))
    self.assertVectorClose(sum2.eval(self.t1), Vector(0,1,1))
    self.assertVectorClose(sum3.eval(self.t1), Vector(1,0,1))

  def test_vector_difference(self):
    diff1 = (self.v1 - self.v2)
    diff2 = (self.v2 - self.v3)
    diff3 = (self.v1 - self.v3)
    self.assertVectorClose(diff1.eval(self.t1), Vector(1,-1,0))
    self.assertVectorClose(diff2.eval(self.t1), Vector(0,1,-1))
    self.assertVectorClose(diff3.eval(self.t1), Vector(1,0,-1))

  def test_vector_amplify(self):
    amp1 = (self.v1 * self.f2)
    amp2 = (self.v2 * self.f2)
    amp3 = (self.v3 * self.f2)
    self.assertVectorClose(amp1.eval(self.t1), Vector(2,0,0))
    self.assertVectorClose(amp2.eval(self.t1), Vector(0,2,0))
    self.assertVectorClose(amp3.eval(self.t1), Vector(0,0,2))

  def test_vector_damp(self):
    damp1 = (self.v1 / self.f2)
    damp2 = (self.v2 / self.f2)
    damp3 = (self.v3 / self.f2)
    self.assertVectorClose(damp1.eval(self.t1), Vector(0.5,0,0))
    self.assertVectorClose(damp2.eval(self.t1), Vector(0,0.5,0))
    self.assertVectorClose(damp3.eval(self.t1), Vector(0,0,0.5))
