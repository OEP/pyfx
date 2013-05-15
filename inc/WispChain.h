#ifndef WISPCHAIN_H_
#define WISPCHAIN_H_

#include <vector>

#include "WispParameters.h"
#include "Box.h"
#include "Noise.h"

namespace vr
{
  class WispChain : public std::vector<WispParameters>
  {
    private:
    protected:
    public:

      const WispParameters interpolate(const double x) const;
      const Box calcBBox(const double noiseMax) const;
  };
}

#endif
