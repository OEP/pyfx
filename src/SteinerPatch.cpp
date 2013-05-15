#include "SteinerPatch.h"

using namespace vr;

const float SteinerPatch::eval(const Vector &p) const
{
  const float x = p[0], y = p[1], z = p[2];
  return -(x*x*y*y + x*x*z*z + y*y*z*z - x*y*z);
}

const Vector SteinerPatch::grad(const Vector &p) const
{
  const float x = p[0], y = p[1], z = p[2];
  return Vector(
    y*z - 2*x*(y*y+z*z),
    -2*x*x*y + x*z - 2*y*z*z,
    x*y - 2*x*x*z - 2*y*y*z);
}

const Box SteinerPatch::getBBox() const
{
  return Box(
    Vector(-0.5,-0.5,-0.5),
    Vector(0.5, 0.5, 0.5));
}
