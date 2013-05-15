#ifndef CLAMP_H_
#define CLAMP_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Clamp : public ScalarField
  {
    private:
      const ScalarField *m_F;
      const float m_E0, m_E1, m_C;

    protected:
    public:
      Clamp(const ScalarField *f, float e0, float e1, float c = 1.0)
        : m_F(f), m_E0(e0), m_E1(e1), m_C(c) {}

      const float eval(const Vector&) const;
      const Box getBBox() const;
      const Vector grad(const Vector&) const;
  };
}

#endif
