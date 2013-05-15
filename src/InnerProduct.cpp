#include "InnerProduct.h"

using namespace vr;

const float InnerProduct::eval(const Vector &p) const
{
  return m_V1->eval(p).dot(m_V2->eval(p));
}

const Vector InnerProduct::grad(const Vector &p) const
{
  return m_V1->grad(p) * m_V2->eval(p) + m_V2->grad(p) * m_V1->eval(p);
}

const Box InnerProduct::getBBox() const
{
  throw std::runtime_error("InnerProduct::getBBox() not implemented");
}
