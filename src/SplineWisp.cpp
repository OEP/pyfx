#include "SplineWisp.h"
#include "macros.h"

using namespace vr;

const Vector SplineWisp::nextSegmentPoint(size_t segment)
{
  float dump;
  return nextSegmentPoint(segment, dump);
}

const Vector SplineWisp::nextSegmentPoint(size_t segment, float &opacity)
{
  const WispParameters &low = (*m_Chain)[segment];
  const WispParameters &hi  = (*m_Chain)[segment+1];
  const Vector
    start = m_PRN.nextVector(Vector(-1, -1, 0), Vector(1, 1, 1));
  const WispParameters params = low.interpolate(hi, start[2]);
  m_FSPN1.setParameters(params.FSPN1);
  m_FSPN2.setParameters(params.FSPN2);
  const double
    denom = HYPOT(start[0], start[1]),
    fspn = pow(fabs(m_FSPN1.eval(start)), params.Clump);
  const Vector
    p = Vector(start[0] * fspn / denom, start[1] * fspn / denom, start[2]),
    pp = low.P0 + // TODO: Is this right?
      p[0] * params.Scale[0] * params.PScale * params.E0 +
      p[1] * params.Scale[1] * params.PScale * params.E1 +
      p[2] * params.E2 * (low.P0 - hi.P0).length() * params.Scale[2],
    t = Vector(m_FSPN2.eval(pp), m_FSPN2.eval(pp + params.Delta), m_FSPN2.eval(pp - params.Delta));
  opacity = params.Opacity;
  return pp + t.componentProduct(params.Scale) * params.PScale;
}

void SplineWisp::addSegmentPoints(const size_t segment)
{
  float opacity;
  const int dots = (*m_Chain)[segment].Dots;
  for(size_t i = 0; i < dots; i++)
  {
    const Vector p = nextSegmentPoint(segment, opacity);
    m_Grid->splat(p, opacity);
  }
}

void SplineWisp::addPoints()
{
  for(size_t i = 0; i < m_Chain->size() - 1; i++)
  {
    addSegmentPoints(i);
  }
}

const Box SplineWisp::calcBBox() const
{
  return m_Chain->calcBBox(m_PN.getMax());
}
