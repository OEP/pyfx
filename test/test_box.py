import vrendtest
import math

from pyfx import Box, Vector

class TestBox(vrendtest.VrendTestCase):

  def setUp(self):
    self.unit = Box(1)
    self.inf = Box.INFINITE
    self.nil = Box.NIL

  def test_infinite(self):
    box = self.unit.intersect(self.inf)
    self.assertBoxClose(box, self.unit)

  def test_nil(self):
    box1 = self.unit.intersect(self.nil)
    box2 = self.unit.expand(self.nil)
    self.assertBoxClose(box1, self.unit)
    self.assertBoxClose(box2, self.unit)

  def test_rotate(self):
    b1 = self.unit.rotate(Vector.UX, math.pi)
    b2 = self.unit.rotate(Vector.UY, math.pi)
    b3 = self.unit.rotate(Vector.UZ, math.pi)
    b4 = self.unit.rotate(Vector.UX, math.pi/4)
    b5 = self.unit.rotate(Vector.UY, math.pi/4)
    b6 = self.unit.rotate(Vector.UZ, math.pi/4)

    for b in (b1, b2, b3):
      self.assertClose(b.sizeX(), 2)
      self.assertClose(b.sizeY(), 2)
      self.assertClose(b.sizeZ(), 2)

    self.assertClose(b4.sizeY(), 2 * math.sqrt(2))
    self.assertClose(b5.sizeZ(), 2 * math.sqrt(2))
    self.assertClose(b6.sizeX(), 2 * math.sqrt(2))

  def test_gridspace(self):
    llc = self.unit.llc()
    res = self.unit.length() / 20
    lower = Vector(0,0,0)
    upper = Vector(1,1,1)

    for k in range(20):
      for j in range(20):
        for i in range(20):
          dv = Vector(i * res.x(), j*res.y(), k*res.z())
          p = llc + dv
          result = self.unit.gridSpace(p)
          self.assertVectorBetween(result, upper, lower)
  
  def test_worldspace(self):
    llc = self.unit.llc()
    urc = self.unit.urc()

    for k in range(21):
      for j in range(21):
        for i in range(21):
          ijk = (x / float(20) for x in (i, j, k))
          gp = Vector(*ijk)
          p = self.unit.worldSpace(gp)
          self.assertVectorBetween(p, urc, llc)

