#include "ScalarReplaceGradient.h"

using namespace vr;

const float ScalarReplaceGradient::eval(const Vector &p) const
{
  return m_F1->eval(p);
}

const Vector ScalarReplaceGradient::grad(const Vector &p) const
{
  return m_V1->eval(p);
}

const Box ScalarReplaceGradient::getBBox() const
{
  return m_F1->getBBox();
}
