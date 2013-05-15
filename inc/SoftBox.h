#ifndef SOFTBOX_H_
#define SOFTBOX_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class SoftBox : public ScalarField
  {
    private:
      const float
        m_R, m_P;
    protected:
    public:
      SoftBox(const float r, const float p)
        : m_R(r), m_P(p) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
