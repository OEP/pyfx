#ifndef ELLIPSE_H_
#define ELLIPSE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Ellipse : public ScalarField
  {
    private:
      const float m_Major, m_Minor;
      const Vector m_N;
    protected:
    public:
      Ellipse(float major, float minor, const Vector &n)
        : m_Major(major), m_Minor(minor), m_N(n.unit()) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
