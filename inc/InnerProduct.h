#ifndef INNERPRODUCT_H_
#define INNERPRODUCT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class InnerProduct : public ScalarField
  {
    private:
      const VectorField *m_V1, *m_V2;

    protected:
    public:
      InnerProduct(const VectorField *v1, const VectorField *v2)
        : m_V1(v1), m_V2(v2) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
