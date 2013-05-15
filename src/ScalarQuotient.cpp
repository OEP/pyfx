#include "ScalarQuotient.h"

using namespace vr;

const float ScalarQuotient::eval(const Vector &p) const
{
  return m_F1->eval(p) / m_F2->eval(p);
}

const Vector ScalarQuotient::grad(const Vector &p) const
{
  throw std::runtime_error("ScalarQuotient::grad(): Not implemented.");
}

const Box ScalarQuotient::getBBox() const
{
  return m_F1->getBBox();
}
