#include "Sphere.h"

using namespace vr;

const Box Sphere::getBBox() const
{
  return Box(m_R);
}

const float Sphere::eval(const Vector &p) const
{
  return m_R - p.length();
}

const Vector Sphere::grad(const Vector &p) const
{
  const float mag = p.length();
  return mag > 0
    ? p / -mag
    : Vector(1., 1., 1.);
}
