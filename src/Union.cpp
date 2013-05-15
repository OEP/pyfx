#include "Union.h"

using namespace vr;

const Box Union::getBBox() const
{
  const Box 
    a = m_VolumeA->getBBox(),
    b = m_VolumeB->getBBox();
  return a.expand(b);
}

const Vector Union::grad(const Vector &p) const
{
  const float
    f1 = m_VolumeA->eval(p),
    f2 = m_VolumeA->eval(p);

  if(f1 > f2)
  {
    return m_VolumeA->grad(p);
  }
  else
  {
    return m_VolumeB->grad(p);
  }
}

const float Union::eval(const Vector &p) const
{
  return std::max(
    m_VolumeA->eval(p),
    m_VolumeB->eval(p));
}
