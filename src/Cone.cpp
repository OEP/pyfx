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
    return m_Theta - acos(xn/p.magnitude());
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
    corner1 = base + r * Vector::AXIS_X,
    corner2 = base + r * Vector::AXIS_Y,
    corner3 = base + r * Vector::AXIS_Z,
    corner4 = base - r * Vector::AXIS_X,
    corner5 = base - r * Vector::AXIS_Y,
    corner6 = base - r * Vector::AXIS_Z,
    corner7 = -r * Vector::AXIS_X,
    corner8 = -r * Vector::AXIS_Y,
    corner9 = -r * Vector::AXIS_Z,
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
