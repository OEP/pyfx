#include "CreateColor.h"

using namespace vr;

const Color CreateColor::eval(const Vector &p) const
{
  return Color(
    evalIf(m_R, p),
    evalIf(m_G, p),
    evalIf(m_B, p),
    evalIf(m_A, p));
}

const float CreateColor::evalIf(const ScalarField *v, const Vector &p) const
{
  return (v != NULL) ? v->eval(p) : 0.0f;
}

/*
const int CreateColor::grad(const Vector &p) const
{
  throw std::exception("Not implemented.");
}
*/

const Box CreateColor::getBBox() const
{
  Box b;
  if(m_R != NULL) b = b.expand(m_R->getBBox());
  if(m_G != NULL) b = b.expand(m_G->getBBox());
  if(m_B != NULL) b = b.expand(m_B->getBBox());
  if(m_A != NULL) b = b.expand(m_A->getBBox());
  return b;
}
