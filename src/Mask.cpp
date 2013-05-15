#include "Mask.h"

using namespace vr;

const float Mask::eval(const Vector &p) const
{
  const float value = m_V->eval(p);
  return (value > 0.0f) ? 1.0f : 0.0f;
}

const Vector Mask::grad(const Vector &p) const
{
  throw std::runtime_error("Mask::grad(): Not implemented.");
}

const Box Mask::getBBox() const
{
  return m_V->getBBox();
}
