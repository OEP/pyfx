#ifndef POWER_H_
#define POWER_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Power : public ScalarField
  {
    private:
      const ScalarField *m_Base;
      const ScalarField *m_Exponent;
    protected:
    public:
      Power(const ScalarField *base, const ScalarField *exponent)
        : m_Base(base), m_Exponent(exponent) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
