#include "Coil.h"

using namespace vr;

const Vector Coil::eval(const Vector &p) const
{
  const Vector
    pp = (p - m_X0),
    xp = (pp.dot(m_Normal) * m_Normal) + m_X0;

  return (p - xp).cross(m_Normal);
}

const Matrix Coil::grad(const Vector &p) const
{
  throw std::runtime_error("Coil::grad(): Not implemented.");
}

const Box Coil::getBBox() const
{
  throw std::runtime_error("Coil::getBBox(): Not implemented.");
}
