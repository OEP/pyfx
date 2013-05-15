#include "Clamp.h"

using namespace vr;

const float Clamp::eval(const Vector &p) const
{
  return std::max(m_E0, std::min(m_E1, m_F->eval(p) / m_C));
}

const Vector Clamp::grad(const Vector &p) const
{
  return m_F->grad(p);
}

const Box Clamp::getBBox() const
{
  return m_F->getBBox();
}
