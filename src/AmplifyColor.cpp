#include "AmplifyColor.h"

using namespace vr;

const Color AmplifyColor::eval(const Vector &p) const
{
  return m_F->eval(p) * m_C->eval(p);
}

const int AmplifyColor::grad(const Vector &p) const
{
  throw std::runtime_error("AmplifyColor::grad(): Not implemented.");
}

const Box AmplifyColor::getBBox() const
{
  return m_C->getBBox().intersect(m_F->getBBox());
}
