#ifndef SCALARPRODUCT_H_
#define SCALARPRODUCT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class ScalarProduct : public ScalarField
  {
    private:
      const ScalarField *m_A, *m_B;
    protected:
    public:
      ScalarProduct(const ScalarField *a, const ScalarField *b)
        : m_A(a), m_B(b) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
