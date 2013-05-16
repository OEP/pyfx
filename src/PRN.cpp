#include "PRN.h"
using namespace vr;

const Vector
PRN::nextVector(const float lo, const float hi)
{
  return nextVector(Vector(lo), Vector(hi));
}

const Vector
PRN::nextVector(const Vector low, const Vector hi)
{
  return Vector(
    low[0] + (hi[0] - low[0]) * eval(),
    low[1] + (hi[1] - low[1]) * eval(),
    low[2] + (hi[2] - low[2]) * eval());
}
