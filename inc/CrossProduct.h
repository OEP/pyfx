#ifndef CROSSPRODUCT_H_
#define CROSSPRODUCT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class CrossProduct : public VectorField
  {
    private:
      const VectorField *m_V1, *m_V2;
    protected:
    public:
      CrossProduct(const VectorField *v1, const VectorField *v2)
        : m_V1(v1), m_V2(v2) {}

      const Vector eval(const Vector&) const;

      //const Matrix grad(const Vector&) const;
      //const Box getBBox() const;
  };
}

#endif
