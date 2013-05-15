#include "StampedNoise.h"

using namespace vr;

const float StampedNoise::eval(const Vector &p) const
{
  const float fs = m_Noise->eval(p);
  return (fs > 0) * fs * pow(m_Fade->eval(p), m_Gamma);
}

const Vector StampedNoise::grad(const Vector &p) const
{
  throw std::runtime_error("StampedNoise::grad(): Not implemented.");
}

const Box StampedNoise::getBBox() const
{
  return m_Fade->getBBox();
}
