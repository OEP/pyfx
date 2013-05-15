#include "ScalarProduct.h"

using namespace vr;

const float ScalarProduct::eval(const Vector &p) const
{
  return m_A->eval(p) * m_B->eval(p);
}

const Vector ScalarProduct::grad(const Vector &p) const
{
  return m_A->grad(p) * m_B->eval(p) +
    m_A->eval(p) * m_B->grad(p);
}

const Box ScalarProduct::getBBox() const
{
  return m_A->getBBox().intersect(m_B->getBBox());
}
