import vrendtest
import math

from vr.vrend import *
from vr.factory import WispFactory

class TestWisp(vrendtest.VrendTestCase):

  def setUp(self):
    self.parameters1 = WispParameters()
    self.parameters1.Dots = 30
    self.wisp1 = Wisp(self.parameters1)

    self.chain1 = WispChain()
    self.chain1.push_back(self.parameters1)

  def test_eval(self):
    self.wisp1.initialize()
    self.wisp1.eval(Vector(0,0,0))

  def test_box_param(self):
    b = Box(10)
    res = Vector(0.01,0.01,0.01)
    wisp = Wisp(self.parameters1, res, 16, b)
    wisp.initialize()
    wisp.eval(Vector(0,0,0))

  def test_chain_bbox(self):
    pn = PerlinNoise()

    p = self.parameters1
    p.P0 += Vector(1, 0, 0)
    wisp2 = Wisp(p)

    box1 = self.wisp1.calcBBox()
    box2 = self.chain1.calcBBox(pn.getMax())

    self.assertVectorClose(box1.llc(), box2.llc())
    self.assertVectorClose(box1.urc(), box2.urc())

    self.chain1.push_back(p)
    box3 = wisp2.calcBBox()
    fullBox = box1.expand(box3)
    testBox = self.chain1.calcBBox(pn.getMax())

    self.assertVectorClose(fullBox.llc(), testBox.llc())
    self.assertVectorClose(fullBox.urc(), testBox.urc())

  def test_parameters(self):
    wp1 = WispParameters()
    wp2 = WispParameters()

    wp1.Opacity *= 2
    wp1.E0 *= 2
    wp1.Dots *= 2
    wp1.FSPN1.roughness *= 2
    self.wisp1.setParameters(wp1)
    self.wisp1.getParameters(wp2)

    self.assertEqual(wp1.Opacity, wp2.Opacity)
    self.assertEqual(wp1.E0, wp2.E0)
    self.assertEqual(wp1.Dots, wp2.Dots)
    self.assertEqual(wp1.FSPN1.roughness, wp2.FSPN1.roughness)
