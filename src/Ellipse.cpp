#include "Ellipse.h"

using namespace vr;

const float Ellipse::eval(const Vector &p) const
{
  const double
    Z = p.dot(m_N);
  const Vector
    xperp = p - Z * m_N;

  return 1 - Z*Z/(m_Major*m_Major) - xperp.dot(xperp)/(m_Minor*m_Minor);
}

const Vector Ellipse::grad(const Vector &p) const
{
  throw std::runtime_error("Ellipse::grad(): Not implemented.");
}

const Box Ellipse::getBBox() const
{
  return Box(m_Major);
}
