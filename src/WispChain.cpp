#include "WispChain.h"
#include "Noise.h"
#include "macros.h"

using namespace vr;


const WispParameters WispChain::interpolate(const double x) const
{
  const int sz = (int) size();
  const double didx = x * (sz - 1);
  const int i = (int) didx;
  const double q = didx - i;

  if(didx < 0)
  {
    return (*this)[0];
  }
  if(i >= sz - 1)
  {
    return (*this)[sz-1];
  }
  return (*this)[i].interpolate((*this)[i+1], q);
}

const Box WispChain::calcBBox(const double noiseMax) const
{
  FractalSum fs1, fs2;
  Box b = Box::NIL;

  for(size_t i = 0; i < size(); i++)
  {
    const WispParameters &p = (*this)[i];
    fs1.setParameters(p.FSPN1);
    fs2.setParameters(p.FSPN2);
    b = b.expand(p.calcBBox(fs1.calcMax(noiseMax), fs2.calcMax(noiseMax)));
  }

  return b;
}
