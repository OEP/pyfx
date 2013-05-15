#include <cmath>

#include "Blinn.h"

using namespace vr;

const float Blinn::eval(const Vector &p) const
{
  const float
    f1 = m_A->eval(p),
    f2 = m_B->eval(p);

  return exp(f1) + exp(f2) - m_C;
}

const Vector Blinn::grad(const Vector &p) const
{
  const float
    f1 = m_A->eval(p),
    f2 = m_B->eval(p);

  return m_A->grad(p) * f1 + m_B->grad(p) * f2;
}

const Box Blinn::getBBox() const
{
  return m_A->getBBox().expand(m_B->getBBox());
}
