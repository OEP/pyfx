#include "PyroclasticSphere.h"

using namespace vr;

const float PyroclasticSphere::eval(const Vector &p) const
{
  const float
    pmag = p.length(),
    fspn = m_Noise->eval(p / pmag);
  return m_Radius - pmag + pow(fabs(fspn), m_Gamma) * m_Amp;
}

const Vector PyroclasticSphere::grad(const Vector &p) const
{
  throw std::runtime_error("PyroclasticSphere::grad(): Not implemented.");
}

const Box PyroclasticSphere::getBBox() const
{
  return Box(m_Radius + m_Noise->getMax());
}
