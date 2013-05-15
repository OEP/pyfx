#include <cmath>

#include "Cone.h"

using namespace vr;

const float Cone::eval(const Vector &p) const
{
  const double xn =  p * m_N;

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
    corner1 = base + r * vector::UX,
    corner2 = base + r * vector::UY,
    corner3 = base + r * vector::UZ,
    corner4 = base - r * vector::UX,
    corner5 = base - r * vector::UY,
    corner6 = base - r * vector::UZ,
    corner7 = -r * vector::UX,
    corner8 = -r * vector::UY,
    corner9 = -r * vector::UZ,
    minp =
      base.componentMin(corner1)
          .componentMin(corner2)
          .componentMin(corner3)
          .componentMin(corner4)
          .componentMin(corner5)
          .componentMin(corner6)
          .componentMin(corner7)
          .componentMin(corner8)
          .componentMin(corner9),
    maxp =
      base.componentMax(corner1)
          .componentMax(corner2)
          .componentMax(corner3)
          .componentMax(corner4)
          .componentMax(corner5)
          .componentMax(corner6)
          .componentMax(corner7)
          .componentMax(corner8)
          .componentMax(corner9);

  return Box(minp, maxp);
}
