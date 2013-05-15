#include "Light.h"

using namespace vr;

const Color Light::color() const
{
  return m_Color * m_Intensity;
}

const Vector& Light::position() const
{
  return m_Position;
}
