#ifndef ICOSAHEDRON_H_
#define ICOSAHEDRON_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Icosahedron : public ScalarField
  {
    private:
    protected:
    public:
      Icosahedron() {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
