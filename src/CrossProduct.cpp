#include "CrossProduct.h"

using namespace vr;

const Vector CrossProduct::eval(const Vector &p) const
{
  return m_V1->eval(p) ^ m_V2->eval(p);
}

/*
const Matrix CrossProduct::grad(const Vector &p) const
{
  throw std::exception("Not implemented.");
}

const Box CrossProduct::getBBox() const
{
  throw std::exception("Not implemented.");
}
*/
