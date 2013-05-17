#include "PerlinNoise.h"


using namespace vr;

const float
PerlinNoise::eval(const Vector &p) const
{
  return m_Noise.GetValue(p.x(), p.y(), p.z());
}

const float
PerlinNoise::getMax() const
{
  return 1.1f;
}
