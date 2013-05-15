#ifndef OUTERPRODUCT_H_
#define OUTERPRODUCT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class OuterProduct : public MatrixField
  {
    private:
      const VectorField *m_V1, *m_V2;
    protected:
    public:
      OuterProduct(const VectorField *v1, const VectorField *v2)
        : m_V1(v1), m_V2(v2) {}

      const Matrix eval(const Vector&) const;

      //const int grad(const Vector&) const;
      //const Box getBBox() const;
  };
}

#endif
