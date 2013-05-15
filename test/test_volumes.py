import vrendtest
import math

from vr.vrend import *

class TestVectorVolumes(vrendtest.VrendTestCase):

  def setUp(self):
    self.v1 = VectorConstant(Vector(1, 0, 0))
    self.v2 = VectorConstant(Vector(0, 1, 0))
    self.v3 = VectorConstant(Vector(0, 0, 1))

    self.f0 = ScalarConstant(0)
    self.f1 = ScalarConstant(1)
    self.f2 = ScalarConstant(2)
    self.f3 = ScalarConstant(3)

    self.t1 = Vector(0,0,0)
    self.t2 = Vector(1,1,1)

  def test_scalar_product(self):
    p1 = ScalarProduct(self.f0, self.f1)
    p2 = ScalarProduct(self.f1, self.f0)
    p3 = ScalarProduct(self.f2, self.f3)
    self.assertEqual(p1.eval(self.t1), 0)
    self.assertEqual(p2.eval(self.t1), 0)
    self.assertEqual(p3.eval(self.t1), 6)
    self.assertVectorClose(p1.grad(self.t1), Vector(0,0,0))
    self.assertVectorClose(p2.grad(self.t1), Vector(0,0,0))
    self.assertVectorClose(p3.grad(self.t1), Vector(0,0,0))

  def test_vector_sum(self):
    sum1 = VectorSum(self.v1, self.v2)
    sum2 = VectorSum(self.v2, self.v3)
    sum3 = VectorSum(self.v1, self.v3)
    self.assertVectorClose(sum1.eval(self.t1), Vector(1,1,0))
    self.assertVectorClose(sum2.eval(self.t1), Vector(0,1,1))
    self.assertVectorClose(sum3.eval(self.t1), Vector(1,0,1))

  def test_vector_difference(self):
    diff1 = VectorDifference(self.v1, self.v2)
    diff2 = VectorDifference(self.v2, self.v3)
    diff3 = VectorDifference(self.v1, self.v3)
    self.assertVectorClose(diff1.eval(self.t1), Vector(1,-1,0))
    self.assertVectorClose(diff2.eval(self.t1), Vector(0,1,-1))
    self.assertVectorClose(diff3.eval(self.t1), Vector(1,0,-1))

  def test_vector_amplify(self):
    amp1 = AmplifyVector(self.v1, self.f2)
    amp2 = AmplifyVector(self.v2, self.f2)
    amp3 = AmplifyVector(self.v3, self.f2)
    self.assertVectorClose(amp1.eval(self.t1), Vector(2,0,0))
    self.assertVectorClose(amp2.eval(self.t1), Vector(0,2,0))
    self.assertVectorClose(amp3.eval(self.t1), Vector(0,0,2))

  def test_vector_damp(self):
    damp1 = DampVector(self.v1, self.f2)
    damp2 = DampVector(self.v2, self.f2)
    damp3 = DampVector(self.v3, self.f2)
    self.assertVectorClose(damp1.eval(self.t1), Vector(0.5,0,0))
    self.assertVectorClose(damp2.eval(self.t1), Vector(0,0.5,0))
    self.assertVectorClose(damp3.eval(self.t1), Vector(0,0,0.5))
