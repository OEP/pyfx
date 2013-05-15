#ifndef FRACTALSUM_H_
#define FRACTALSUM_H_

#include "Vector.h"
#include "Noise.h"
#include "FractalSumParameters.h"

namespace vr
{


  class FractalSum : public Noise
  {
    private:
    protected:
    public:
      FractalSum(const Noise *n = NULL,
        const FractalSumParameters params = FractalSumParameters());

      void setParameters(const FractalSumParameters &p);
      const double calcMax(const double) const;

      virtual const float eval(const Vector &p) const;
      virtual const float getMax() const;
  };
}

#endif
