#include "FractalSum.h"
using namespace vr;

#include <cmath>

void
FractalSum::setParameters(const FractalSumParameters &p)
{
  m_Params = p;
}

const double
FractalSum::calcMax(const double noiseMax) const
{
  return noiseMax * (1 - pow(m_Params.roughness, m_Params.octaves)) / (1 - m_Params.roughness);
}

const float
FractalSum::eval(const Vector &p) const
{
  float value = 0.0f;

  size_t j = 0;
  for(; j+1 < m_Params.octaves; j++)
  {
    value += evalIteration(p, j);
  }

  const float wt = m_Params.octaves - j;

  return value + (1-wt) * evalIteration(p, j) + wt * evalIteration(p, j+1);
}

const float
FractalSum::evalIteration(const Vector &p, size_t j) const
{
  const float f = m_Params.frequency * pow(m_Params.fjump, j);
  return pow(m_Params.roughness, j) *
    m_Noise->eval((p - m_Params.translate) * f + m_Params.offset);
}

const float
FractalSum::getMax() const
{
  return calcMax(m_Noise->getMax());
}
