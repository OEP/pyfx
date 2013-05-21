#ifndef NOISE_H_
#define NOISE_H_

#include "Vector.h"
#include "Volume.h"
#include "NoiseParameters.h"

namespace vr
{
  class Noise : public ScalarField
  {
    private:
    protected:
    public:

      virtual const float eval(const Vector &p) const = 0;
      virtual const float getMax() const = 0;
      virtual const Box getBBox() const;
      virtual const Vector grad(const Vector &p) const;
  };
}

#endif
