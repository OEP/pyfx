#include "AbsoluteValue.h"

#include <cstdlib>

using namespace vr;

const float AbsoluteValue::eval(const Vector &p) const
{
  return fabs(m_F->eval(p));
}

const Vector AbsoluteValue::grad(const Vector &p) const
{
  throw std::runtime_error("AbsoluteValue::grad(): Not implemented.");
}

const Box AbsoluteValue::getBBox() const
{
  return m_F->getBBox();
}
