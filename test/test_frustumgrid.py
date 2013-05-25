import vrendtest
import math

from pyfx import *

class TestFrustumGrid(vrendtest.VrendTestCase):

  GRID_RES = 10

  def setUp(self):
    self.c1 = Camera()
    self.c1.look(2,0,0,0,0,0,0,1,0)
    self.c1.setNearPlane(1)
    self.c1.setFarPlane(4)

    self.c2 = Camera()
    self.c2.look(0,0,0,0,0,-1,0,1,0)
    self.c2.setNearPlane(1)
    self.c2.setFarPlane(2)

    self.sphere1 = Sphere()


    self.res1 = self.c1.computeResolution(TestFrustumGrid.GRID_RES)
    self.grid1 = DenseGrid(self.c1, self.res1, 0)
    self.grid2 = SparseGrid(self.c1, self.res1, 0, 16)

    self.grid1.stamp(self.sphere1)
    self.grid2.stamp(self.sphere1)


    self.b1 = Box(Vector(0,0,0), Vector(1,1,1))
    self.r1 = Vector(0.1,0.1,0.1)
    self.r2 = Vector(0.09,0.09,0.09)

  def test_spacetransform(self):
    for k in range(TestFrustumGrid.GRID_RES+1):
      for j in range(TestFrustumGrid.GRID_RES+1):
        for i in range(TestFrustumGrid.GRID_RES+1):
          pi = i / float(TestFrustumGrid.GRID_RES)
          pj = j / float(TestFrustumGrid.GRID_RES)
          pk = k / float(TestFrustumGrid.GRID_RES)
          test = Vector(pi,pj,pk)
          ws = self.c1.worldSpace(test)
          gs = self.c1.gridSpace(ws)
          self.assertVectorClose(gs, test)

  def test_worldspace(self):
    goal = Vector(0, 0, -2)
    p1 = self.c2.worldSpace(Vector(0.5,0.5,1))
    self.assertVectorClose(p1, goal)

  def test_gridspace(self):
    upper = Vector(1.01,1.01,1.01)
    lower = Vector(-0.01,-0.01,-0.01)
    for k in range(TestFrustumGrid.GRID_RES+1):
      for j in range(TestFrustumGrid.GRID_RES+1):
        for i in range(TestFrustumGrid.GRID_RES+1):
          pi = i / float(TestFrustumGrid.GRID_RES)
          pj = j / float(TestFrustumGrid.GRID_RES)
          pk = k / float(TestFrustumGrid.GRID_RES)
          r = self.c1.getRay(pi, pj)
          s = pk * self.c1.viewLength()
          tp = r.trace(s)
          p = self.c1.gridSpace(tp)
          self.assertVectorBetween(p, upper, lower)

