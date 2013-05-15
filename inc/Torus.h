#ifndef TORUS_H_
#define TORUS_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class Torus : public ScalarField
  {
    private:
      const float
        m_Major, m_Minor;

      const Vector
        m_Norm;
    protected:
    public:
      Torus(float major, float minor, const Vector norm)
        : m_Major(major), m_Minor(minor), m_Norm(norm) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
