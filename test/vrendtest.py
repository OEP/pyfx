
import unittest

class VrendTestCase(unittest.TestCase):

  DEFAULT_DELTA = 0.001

  def assertClose(self, test, expected):
    self.assertWithin(test, expected, VrendTestCase.DEFAULT_DELTA)

  def assertBoxClose(self, a, b):
    self.assertVectorClose(a.llc(), b.llc())
    self.assertVectorClose(a.urc(), b.urc())

  def assertVectorBetween(self, test, upper, lower):
    mid = (upper + lower) / 2
    length = upper - lower
    dx = length.x() / 2
    dy = length.y() / 2
    dz = length.z() / 2

    try:
      self.assertWithin(test.x(), mid.x(), dx)
      self.assertWithin(test.y(), mid.y(), dy)
      self.assertWithin(test.z(), mid.z(), dz)
    except AssertionError:
      args = (test, lower, upper)
      raise AssertionError("%s not between %s and %s" % args)

  def assertWithin(self, test, expected, delta):
    try:
      self.assertLessEqual(test, expected + delta)
      self.assertGreaterEqual(test, expected - delta)
    except AssertionError:
      raise AssertionError("%f not in %f +/- %f" %
        (test, expected, delta))

  def assertVectorWithin(self, test, expected, delta):
    try:
      self.assertWithin(test.x(), expected.x(), delta)
      self.assertWithin(test.y(), expected.y(), delta)
      self.assertWithin(test.z(), expected.z(), delta)
    except AssertionError:
      raise AssertionError("%s not in %s +/- %f" %
        str(test), str(expected), delta)

  def assertVectorClose(self, test, expected):
    self.assertVectorWithin(test, expected, VrendTestCase.DEFAULT_DELTA)
