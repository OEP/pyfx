#include "PerlinNoise.h"


using namespace vr;

ScalarField::Ptr PerlinNoise::create()
{
  return ScalarField::Ptr(new PerlinNoise());
}

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
