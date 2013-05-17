#ifndef PERLINNOISE_H_
#define PERLINNOISE_H_

#include "Noise.h"
#include <libnoise/noise.h>

namespace vr
{
  class PerlinNoise : public Noise
  {
    private:
      const noise::module::Perlin m_Noise;
    protected:
    public:
      virtual const float eval(const Vector &p) const;
      virtual const float getMax() const;
  };
}

#endif
