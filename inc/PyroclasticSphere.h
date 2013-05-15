#ifndef PYROCLASTICSPHERE_H_
#define PYROCLASTICSPHERE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Noise.h"

namespace vr
{
  class PyroclasticSphere : public ScalarField
  {
    private:
      const Noise *m_Noise;
      const float m_Radius;
      const float m_Gamma;
      const float m_Amp;
    protected:
    public:
      PyroclasticSphere(const Noise *noise,
          const float radius = 1.0, const float gamma = 1.0,
          const float amp = 1.0)
        : m_Noise(noise),
          m_Radius(radius),
          m_Gamma(gamma),
          m_Amp(amp) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
