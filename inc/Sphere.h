#ifndef IMPLICITSPHERE_H_
#define IMPLICITSPHERE_H_

#include "Volume.h"

namespace vr
{
  class Sphere : public ScalarField
  {
    private:
      const float m_R;
    protected:
    public:
      Sphere(float r=1) : m_R(r) {}

      const Box getBBox() const;
      const Vector grad(const Vector&) const;
      const float eval(const Vector&) const;
  };
}

#endif
