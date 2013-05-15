#ifndef SCALARQUOTIENT_H_
#define SCALARQUOTIENT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class ScalarQuotient : public ScalarField
  {
    private:
      const ScalarField *m_F1, *m_F2;
    protected:
    public:
      ScalarQuotient(const ScalarField *f1, const ScalarField *f2)
        : m_F1(f1), m_F2(f2) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
