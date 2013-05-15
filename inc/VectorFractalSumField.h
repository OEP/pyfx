#ifndef VECTORFRACTALSUMFIELD_H_
#define VECTORFRACTALSUMFIELD_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "FractalSum.h"

namespace vr
{
  class VectorFractalSumField : public VectorField
  {
    private:
      const FractalSum *m_FS;
      const Vector m_V1;
      const Vector m_V2;
    protected:
    public:
      VectorFractalSumField(const FractalSum *fspn, const Vector &v1, const
        Vector &v2)
        : m_FS(fspn), m_V1(v1), m_V2(v2) {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
