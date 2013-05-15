#ifndef IDENTITY_H_
#define IDENTITY_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Identity : public VectorField
  {
    private:
    protected:
    public:
      Identity() {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
