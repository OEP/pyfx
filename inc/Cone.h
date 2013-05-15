#ifndef CONE_H_
#define CONE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Cone : public ScalarField
  {
    private:
      const Vector m_N;
      const float m_H, m_Theta;
    protected:
    public:
      Cone(const Vector n, const float h, const float theta)
        : m_N(n.unit()), m_H(h), m_Theta(theta) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
