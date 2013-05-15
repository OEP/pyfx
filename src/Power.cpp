#include "Power.h"
#include <cmath>

using namespace vr;

const float Power::eval(const Vector &p) const
{
  return pow(m_Base->eval(p), m_Exponent->eval(p));
}

const Vector Power::grad(const Vector &p) const
{
  throw std::runtime_error("Power::grad(): Not implemented.");
}

const Box Power::getBBox() const
{
  return m_Base->getBBox();
}
