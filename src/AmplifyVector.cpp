#include "AmplifyVector.h"

using namespace vr;

const Vector AmplifyVector::eval(const Vector &p) const
{
  return m_V1->eval(p) * m_F1->eval(p);
}

const Matrix AmplifyVector::grad(const Vector &p) const
{
  throw std::runtime_error("AmplifyVector::grad() Not implemented.");
}

const Box AmplifyVector::getBBox() const
{
  return m_V1->getBBox().intersect(m_F1->getBBox());
}
