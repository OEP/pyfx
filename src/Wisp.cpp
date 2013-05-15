#include "Wisp.h"

using namespace vr;

const Box Wisp::calcBBox() const
{
  return m_Parameters.calcBBox(m_FSPN1.getMax(), m_FSPN2.getMax());
}

const Vector Wisp::nextPoint()
{
  const Vector start = m_PRN.nextVector(-1, 1);
  const Vector y = start / start.length();
  const float fspn1 = fabs(m_FSPN1.eval(start));
  const float r = pow(fspn1, m_Parameters.Clump);
  const Vector yp = y * r;
  const Vector p = m_Parameters.P0 +
   (m_Parameters.Scale[0] * m_Parameters.E0 * yp[0] +
    m_Parameters.Scale[1] * m_Parameters.E1 * yp[1] +
    m_Parameters.Scale[2] * m_Parameters.E2 * yp[2]) * m_Parameters.PScale;
  const Vector t = Vector(
    m_FSPN2.eval(p),
    m_FSPN2.eval(p + m_Parameters.Delta),
    m_FSPN2.eval(p - m_Parameters.Delta));
  return p + t;
}

void Wisp::addPoint()
{
  m_Grid->splat(nextPoint(), m_Parameters.Opacity);
}

void Wisp::addPoints()
{
  for(int i = 0; i < m_Parameters.Dots; i++)
  {
    addPoint();
  }
}

