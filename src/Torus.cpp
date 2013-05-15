#include "Torus.h"

using namespace vr;

const float Torus::eval(const Vector &p) const
{
  const Vector xperp = p - (p * m_Norm) * m_Norm;
  const float
    left = 4 * m_Major * m_Major * (xperp * xperp),
    right = ((p * p) + m_Major * m_Major - m_Minor * m_Minor);
  return left - right * right; 
}

const Vector Torus::grad(const Vector &p) const
{
  throw std::runtime_error("Torus::grad(): Not implemented.");
}

const Box Torus::getBBox() const
{
  return Box(m_Major + m_Minor);
}
