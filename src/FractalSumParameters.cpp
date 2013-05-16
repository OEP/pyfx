#include "FractalSumParameters.h"
#include "macros.h"
using namespace vr;

const FractalSumParameters
FractalSumParameters::interpolate(const FractalSumParameters &other, float q) const
{
  const float q0 = 1.0f - q;
  FractalSumParameters out;

  out.octaves      = LERP(octaves, other.octaves, q);
  out.fjump        = LERP(fjump, other.fjump  , q);
  out.roughness    = LERP(roughness, other.roughness, q);
  out.frequency    = LERP(frequency, other.frequency, q);
  out.translate    = LERP(translate, other.translate, q);
  out.offset       = LERP(offset, other.offset, q);
  out.axis         = LERP(axis, other.axis, q);
  out.angle        = LERP(angle, other.angle, q);

  return out;
}
