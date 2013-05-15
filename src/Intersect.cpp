#include "Intersect.h"

using namespace vr;

const Box Intersect::getBBox() const
{
  const Box 
    a = m_VolumeA->getBBox(),
    b = m_VolumeB->getBBox();
  return a.intersect(b);
}

const Vector Intersect::grad(const Vector &p) const
{
  const float
    f1 = m_VolumeA->eval(p),
    f2 = m_VolumeA->eval(p);

  if(f1 < f2)
  {
    return m_VolumeA->grad(p);
  }
  else
  {
    return m_VolumeB->grad(p);
  }
}

const float Intersect::eval(const Vector &p) const
{
  return std::min(
    m_VolumeA->eval(p),
    m_VolumeB->eval(p));
}
