#include "DampVector.h"

using namespace vr;

const Vector DampVector::eval(const Vector &p) const
{
  return m_V1->eval(p) / m_F1->eval(p);
}

/*
const Matrix DampVector::grad(const Vector &p) const
{
  throw std::exception("Not implemented.");
}

const Box DampVector::getBBox() const
{
  throw std::exception("Not implemented.");
}
*/
