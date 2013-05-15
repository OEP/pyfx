#ifndef PERLINNOISE_H_
#define PERLINNOISE_H_

#include "Noise.h"

namespace vr
{
  class PerlinNoise : public Noise
  {
    private:
    protected:
    public:
      virtual const float eval(const Vector &p) const;
      virtual const float getMax() const;
  };
}

#endif
