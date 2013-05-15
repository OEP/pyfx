#ifndef WISPPARAMETERS_H_
#define WISPPARAMETERS_H_

#include "Box.h"
#include "FractalSum.h"

namespace vr
{
  class WispParameters
  {
    private:
    protected:
    public:
      FractalSumParameters
        FSPN1,
        FSPN2;

      Vector
        E0,
        E1,
        E2,
        P0,
        Scale,
        Delta;

      float
        Opacity,
        PScale,
        Clump,
        Amp;

      int
        Dots;

      WispParameters() :
        FSPN1   (FractalSumParameters()),
        FSPN2   (FractalSumParameters()),
        E0      (vector::UX),
        E1      (vector::UY),
        E2      (vector::UZ),
        P0      (Vector::ZEROS),
        Scale   (Vector::ONES),
        Delta   (Vector::ZEROS),
        Opacity (1.0f),
        PScale  (1.0f),
        Clump   (0.33f),
        Amp     (1.0f),
        Dots    (1)
        {}

      void interpolate(const WispParameters &other, float q, WispParameters &result) const;
      const WispParameters interpolate(const WispParameters &other, float q) const;
      const Box calcBBox(const double, const double) const;
  };
}

#endif
