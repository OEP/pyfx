#ifndef PLANE_H_
#define PLANE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Plane : public ScalarField
  {
    private:
      const Vector
        m_X0, m_N;
    protected:
    public:
      Plane(const Vector n, const Vector x0=Vector(0,0,0))
        : m_X0(x0), m_N(n.unit()) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
