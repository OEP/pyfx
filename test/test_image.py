import vrendtest
import math

from pyfx import *

class TestImage(vrendtest.VrendTestCase):

  def setUp(self):
    self.im = Image(2, 2, 1)
    self.im.setChannel(0, 0, 0, 1)
    
    self.p00 = Vector2D(0,0)
    self.p01 = Vector2D(0,1)
    self.p10 = Vector2D(1,0)
    self.p11 = Vector2D(1,1)

  def test_read(self):
    self.assertEqual(self.im.read(0, 0, 0), 1)
    
  def test_set(self):
    self.im.set(0, 0, 2)
    self.assertEqual(self.im.read(0, 0, 0), 2)

  def test_add(self):
    def average(x): return sum(x) / len(x)
    to_add = (1, 3, 6, 1, 7, 8, 1, 6, 1, 3)
    added = []

    for x in to_add:
      self.im.add(0, 1, x)
      added.append(x)

      self.assertClose(self.im.read(0, 1, 0), average(added))

  def test_in_bounds(self):
    self.assertTrue(self.im.inBounds(0, 0, 0))
    self.assertTrue(self.im.inBounds(0, 1, 0))
    self.assertTrue(self.im.inBounds(1, 0, 0))
    self.assertTrue(self.im.inBounds(1, 1, 0))
    
    self.assertFalse(self.im.inBounds(0, 0, 1))
    self.assertFalse(self.im.inBounds(1, 2, 0))

  def test_evalChannel(self):
    self.assertTrue(self.im.inBounds(0, 0, 0))
    self.assertTrue(self.im.inBounds(0, 1, 0))
    self.assertTrue(self.im.inBounds(1, 0, 0))
    self.assertTrue(self.im.inBounds(1, 1, 0))
    self.assertEqual(self.im.evalChannel(0, 0, 0), 1)
    self.assertEqual(self.im.evalChannel(0, 1, 0), 0)
    self.assertEqual(self.im.evalChannel(1, 0, 0), 0)
    self.assertEqual(self.im.evalChannel(1, 1, 0), 0)

    self.assertEqual(self.im.evalChannel(self.p00, 0), 1)
    self.assertEqual(self.im.evalChannel(self.p01, 0), 0)
    self.assertEqual(self.im.evalChannel(self.p10, 0), 0)
    self.assertEqual(self.im.evalChannel(self.p11, 0), 0)
