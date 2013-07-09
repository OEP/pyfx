#ifndef CREATEVECTOR_H_
#define CREATEVECTOR_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class CreateVector : public VectorField
  {
    private:
      const ScalarField *m_X, *m_Y, *m_Z;
    protected:
    public:
      CreateVector(
        const ScalarField *x = NULL,
        const ScalarField *y = NULL,
        const ScalarField *z = NULL)
        : m_X(x), m_Y(y), m_Z(z) {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
