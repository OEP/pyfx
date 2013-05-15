#ifndef GRADIENT_H_
#define GRADIENT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Gradient : public VectorField
  {
    private:
      const ScalarField *m_V;
    protected:
    public:
      Gradient(const ScalarField *v)
        : m_V(v) {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
