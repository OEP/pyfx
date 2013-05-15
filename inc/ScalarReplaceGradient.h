#ifndef SCALARREPLACEGRADIENT_H_
#define SCALARREPLACEGRADIENT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class ScalarReplaceGradient : public ScalarField
  {
    private:
      const ScalarField *m_F1;
      const VectorField *m_V1;
    protected:
    public:
      ScalarReplaceGradient(const ScalarField *f1, const VectorField *v1)
        : m_F1(f1), m_V1(v1) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
