#ifndef NOISE_H_
#define NOISE_H_

#include "Vector.h"

namespace vr
{
  class NoiseParameters
  {
    public:
  };

  class Noise
  {
    private:
    protected:
    public:

      virtual const float eval(const Vector &p) const;
      virtual const float getMax() const;
      virtual const float setParameters(const NoiseParameters &p);
  };
}

#endif
