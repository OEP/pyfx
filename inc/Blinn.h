#ifndef BLINN_H_
#define BLINN_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Blinn : public ScalarField
  {
    private:
      const ScalarField
        *m_A, *m_B;

      const float m_C;
    protected:
    public:
      Blinn(const ScalarField *a, const ScalarField *b, const float c=0.0f)
        : m_A(a), m_B(b), m_C(c){}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
