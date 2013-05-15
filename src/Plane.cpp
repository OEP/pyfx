#include "Plane.h"

using namespace vr;

const float Plane::eval(const Vector &p) const
{
  return (p - m_X0) * m_N;
}

const Vector Plane::grad(const Vector &p) const
{
  throw std::runtime_error("Plane::grad(): Not implemented.");
}

const Box Plane::getBBox() const
{
  return Box::INFINITE;
}
