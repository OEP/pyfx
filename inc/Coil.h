#ifndef COIL_H_
#define COIL_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Coil : public VectorField
  {
    private:
    protected:
      const Vector m_Normal;
      const Vector m_X0;
    public:
      Coil(
        const Vector &n,
        const Vector &x0=Vector(0))
        :
        m_Normal(n.unit()),
        m_X0(x0)
       {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
