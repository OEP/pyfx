#ifndef MASK_H_
#define MASK_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Mask : public ScalarField
  {
    private:
      const ScalarField *m_V;
    protected:
    public:
      Mask(const ScalarField *v)
        : m_V(v) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
