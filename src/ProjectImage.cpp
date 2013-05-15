#include "ProjectImage.h"

using namespace vr;

const float ProjectImage::evalPlane(const Vector2D &p) const
{
  return 2.0f * (m_Image->evalChannel(p, m_Channel) - 0.5);
}

