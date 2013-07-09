#ifndef COLORCOMPONENT_H_
#define COLORCOMPONENT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class ColorComponent : public ScalarField
  {
    private:
      const ColorField *m_C;
      const int m_Index;
    protected:
    public:
      ColorComponent(const ColorField *c, const int i)
        : m_C(c), m_Index(i) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
