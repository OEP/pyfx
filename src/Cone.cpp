#include <cmath>

#include "Cone.h"

using namespace vr;

const float Cone::eval(const Vector &p) const
{
  const double xn = p.dot(m_N);

  if(xn < 0)
  {
    return xn;
  }
  else if(xn > m_H)
  {
    return m_H - xn;
  }
  else
  {
    return m_Theta - acos(xn/p.length());
  }
}

const Vector Cone::grad(const Vector &p) const
{
  throw std::runtime_error("Cone::grad(): Not implemented.");
}

const Box Cone::getBBox() const
{
  const double
    r = m_H * tan(m_Theta);
  const Vector
    base = m_H * m_N,
    corner1 = base + r * Vector::UX,
    corner2 = base + r * Vector::UY,
    corner3 = base + r * Vector::UZ,
    corner4 = base - r * Vector::UX,
    corner5 = base - r * Vector::UY,
    corner6 = base - r * Vector::UZ,
    corner7 = -r * Vector::UX,
    corner8 = -r * Vector::UY,
    corner9 = -r * Vector::UZ,
    minp =
      base.minComponent(corner1)
        .minComponent(corner2)
        .minComponent(corner3)
        .minComponent(corner4)
        .minComponent(corner5)
        .minComponent(corner6)
        .minComponent(corner7)
        .minComponent(corner8)
        .minComponent(corner9),
    maxp =
      base.maxComponent(corner1)
        .maxComponent(corner2)
        .maxComponent(corner3)
        .maxComponent(corner4)
        .maxComponent(corner5)
        .maxComponent(corner6)
        .maxComponent(corner7)
        .maxComponent(corner8)
        .maxComponent(corner9);

  return Box(minp, maxp);
}
