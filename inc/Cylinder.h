#ifndef CYLINDER_H_
#define CYLINDER_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Cylinder : public ScalarField
  {
    private:
      const Vector m_N;
      const double m_R, m_L;
    protected:
    public:
      Cylinder(const Vector n, const double r=1.0, const double l=-1.0)
        : m_N(n.unit()), m_R(r), m_L(l) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
