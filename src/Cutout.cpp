#include "Cutout.h"

using namespace vr;

const Box Cutout::getBBox() const
{
  return m_VolumeA->getBBox();
}

const Vector Cutout::grad(const Vector &p) const
{
  const float
    f1 = m_VolumeA->eval(p),
    nf2 = -m_VolumeB->eval(p);

  if(f1 < nf2)
  {
    return m_VolumeA->grad(p);
  }
  else
  {
    return m_VolumeB->grad(p) * -1;
  }
}

const float Cutout::eval(const Vector &p) const
{
  return std::min(
    m_VolumeA->eval(p),
    -m_VolumeB->eval(p));
}
