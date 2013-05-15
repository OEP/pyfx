#ifndef NORMALIZE_H_
#define NORMALIZE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Normalize : public VectorField
  {
    private:
    protected:
      const VectorField *m_Field;
    public:
      Normalize(const VectorField *v)
        : m_Field(v) {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
