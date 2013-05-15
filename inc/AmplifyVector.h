#ifndef VECTORTIMESSCALAR_H_
#define VECTORTIMESSCALAR_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class AmplifyVector : public VectorField
  {
    private:
      const VectorField *m_V1;
      const ScalarField *m_F1;
    protected:
    public:
      AmplifyVector(const VectorField *v1, const ScalarField *f1)
        : m_V1(v1), m_F1(f1) {}

      const Vector eval(const Vector&) const;

      const Matrix grad(const Vector&) const;

      const Box getBBox() const;
  };
}

#endif
