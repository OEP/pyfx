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
  using openvdb::OPENVDB_VERSION_NAME::math::minComponent;
  using openvdb::OPENVDB_VERSION_NAME::math::maxComponent;

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
      minComponent(base,
        minComponent(corner1,
        minComponent(corner2,
        minComponent(corner3,
        minComponent(corner4,
        minComponent(corner5,
        minComponent(corner6,
        minComponent(corner7,
        minComponent(corner8, corner9))))))))),
    maxp =
      maxComponent(base,
        maxComponent(corner1,
        maxComponent(corner2,
        maxComponent(corner3,
        maxComponent(corner4,
        maxComponent(corner5,
        maxComponent(corner6,
        maxComponent(corner7,
        maxComponent(corner8, corner9)))))))));

  return Box(minp, maxp);
}
