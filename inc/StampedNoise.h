#ifndef STAMPEDNOISE_H_
#define STAMPEDNOISE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Noise.h"

namespace vr
{
  class StampedNoise : public ScalarField
  {
    private:
      const Noise *m_Noise;
      const ScalarField *m_Fade;
      const float m_Gamma;
    protected:
    public:
      StampedNoise(const Noise *noise, const ScalarField *fade, const float gamma = 1.0f)
        : m_Noise(noise), m_Fade(fade), m_Gamma(gamma) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
