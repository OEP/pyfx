#ifndef AMPLIFYCOLOR_H_
#define AMPLIFYCOLOR_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class AmplifyColor : public ColorField
  {
    private:
      const ColorField *m_C;
      const ScalarField *m_F;
    protected:
    public:
      AmplifyColor(const ColorField *c, const ScalarField *f)
        : m_C(c), m_F(f) {}

      const Color eval(const Vector&) const;
      const int grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
