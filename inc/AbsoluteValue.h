#ifndef ABSOLUTEVALUE_H_
#define ABSOLUTEVALUE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class AbsoluteValue : public ScalarField
  {
    private:
      const ScalarField *m_F;
    protected:
    public:
      AbsoluteValue(const ScalarField *f)
        : m_F(f) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
