#include "PointWispWanderer.h"
using namespace vr;
const Vector PointWispWanderer::nextPoint()
{
  const Vector start = m_PRN.nextVector(-1, 1);
  const Vector y = start / start.length();
  const float fspn1 = fabs(m_Parameters.FSPN1.eval(start));
  const float r = pow(fspn1, m_Parameters.Clump);
  const Vector yp = y * r;
  const Vector p = m_Parameters.P0 +
   (m_Parameters.Scale[0] * m_Parameters.E0 * yp[0] +
    m_Parameters.Scale[1] * m_Parameters.E1 * yp[1] +
    m_Parameters.Scale[2] * m_Parameters.E2 * yp[2]) * m_Parameters.PScale;
  const Vector t = Vector(
    m_Parameters.FSPN2.eval(p),
    m_Parameters.FSPN2.eval(p + m_Parameters.Delta),
    m_Parameters.FSPN2.eval(p - m_Parameters.Delta));
  return p + t;
}

void PointWispWanderer::setSeed(const unsigned long s)
{
  m_PRN.setSeed(s);
}

void PointWispWanderer::setParameters(const WispParameters &p)
{
  m_Parameters = p;
}
