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
      const Noise *m_Noise;
      FractalSumParameters m_Params;
    protected:
    public:
      FractalSum(const Noise *n = NULL,
        const FractalSumParameters params = FractalSumParameters()):
        m_Noise(n), m_Params(params) {};

      void setParameters(const FractalSumParameters &p);
      const double calcMax(const double) const;

      virtual const float eval(const Vector &p) const;
      virtual const float getMax() const;
  };
}

#endif
