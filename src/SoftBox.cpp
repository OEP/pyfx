#include <cmath>

#include "SoftBox.h"

using namespace vr;

const float SoftBox::eval(const Vector &p) const
{
  const float
    x = p[0],
    y = p[1],
    z = p[2];
  return m_R - pow(x,m_P) - pow(y,m_P) - pow(z,m_P);
}

const Vector SoftBox::grad(const Vector &p) const
{
  throw std::runtime_error("SoftBox::grad(): Not implemented.");
}

const Box SoftBox::getBBox() const
{
  return Box(
    Vector(-m_R, -m_R, -m_R),
    Vector(m_R, m_R, m_R));
}
