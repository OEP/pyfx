import vrendtest
import math

from vr.vrend import *
from vr.graph import VolumeGraph as vg

class TestVolumeGraph(vrendtest.VrendTestCase):

  def setUp(self):
    self.tree1 = Sphere
    self.tree2 = (Translate, Vector(5, 0, 0), Sphere)

    self.g1 = vg.fromtree(self.tree1)
    self.g2 = vg.fromtree(self.tree2)

  def test_merge(self):
    old = self.g1.top()
    self.g1.merge(self.g2)

    for i in self.g2._instances:
      self.assertTrue(i in self.g1._instances)

    self.assertTrue(old in self.g1._instances)
    self.assertEqual(self.g1.top(), self.g2.top())

  def test_fromtree(self):
    self.assertGreater(self.g1.eval(Vector(0,0,0)), 0)
    self.assertLess(self.g1.eval(Vector(2,0,0)), 0)

    self.assertGreater(self.g2.eval(Vector(5,0,0)), 0)
    self.assertLess(self.g2.eval(Vector(2,0,0)), 0)

    

  def test_treemerge(self):
    g = vg.fromtree((Union, self.g1, self.g2))

    self.assertTrue(isinstance(g.top(), Union))

    self.assertGreater(g.eval(Vector(0,0,0)), 0)
    self.assertGreater(g.eval(Vector(5,0,0)), 0)
    self.assertLess(g.eval(Vector(3,0,0)), 0)
