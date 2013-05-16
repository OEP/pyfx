#include "BoxedCutter.h"

using namespace vr;

const float BoxedCutter::eval(int i, const Vector &p) const
{
  return getBBox(i).contains(p);
}

const Vector BoxedCutter::grad(int i, const Vector &p) const
{
  throw std::runtime_error("BoxedCutter::grad(): Not implemented.");
}

const Box BoxedCutter::getBBox(int i) const
{
  Vector ndx(i % (int)(nUnits[0]), (i % (int)(nUnits[0] * nUnits[1])) / (int)(nUnits[1]), i / (int)(nUnits[0] * nUnits[1]));
  return Box(ndx.scale(res) + bBox.llc(),
             ndx.scale(res) + bBox.llc() + res);
}
