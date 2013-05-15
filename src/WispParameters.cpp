#include "WispParameters.h"
#include "macros.h"

using namespace vr;


void WispParameters::interpolate
(const WispParameters &other, float q, WispParameters &result) const
{
  result.FSPN1   = FSPN1.interpolate(other.FSPN1, q);
  result.FSPN2   = FSPN2.interpolate(other.FSPN2, q);
  result.E0      = vector::lerpRotate(E0, other.E0, q);
  result.E1      = vector::lerpRotate(E1, other.E1, q);
  result.E2      = vector::lerpRotate(E2, other.E2, q);
  result.P0      = LERP(P0, other.P0, q);
  result.Scale   = LERP(Scale, other.Scale, q);
  result.Delta   = LERP(Delta, other.Delta, q);
  result.Opacity = LERP(Opacity, other.Opacity, q);
  result.PScale  = LERP(PScale, other.PScale, q);
  result.Clump   = LERP(Clump, other.Clump, q);
  result.Amp     = LERP(Amp, other.Amp, q);
  result.Dots    = Dots;
}

const WispParameters WispParameters::interpolate
(const WispParameters &other, float q) const
{
  WispParameters out;
  interpolate(other, q, out);
  return out;
}

const Box WispParameters::calcBBox(const double fs1max, const double fs2max) const
{
  return Box(pow(fs1max, Clump) + fs2max)
    .scale(PScale * Scale)
    .translate(P0);
}
