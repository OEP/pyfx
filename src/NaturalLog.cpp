#include<cmath>

#include "NaturalLog.h"

using namespace vr;

const float NaturalLog::eval(const Vector &p) const
{
  return logf(m_F1->eval(p));
}

const Vector NaturalLog::grad(const Vector &p) const
{
  return m_F1->grad(p) / m_F1->eval(p);
}

const Box NaturalLog::getBBox() const
{
  return m_F1->getBBox();
}
