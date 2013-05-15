#ifndef CREATECOLOR_H_
#define CREATECOLOR_H_

#include "Volume.h"
#include "Color.h"

namespace vr
{
  class CreateColor : public ColorField
  {
    private:
      const ScalarField
        *m_R, *m_G, *m_B, *m_A;

      const float evalIf(const ScalarField*, const Vector &) const;
    protected:
    public:
      CreateColor(
        const ScalarField *r = NULL,
        const ScalarField *g = NULL,
        const ScalarField *b = NULL, 
        const ScalarField *a = NULL)
        : m_R(r), m_G(g), m_B(b), m_A(a) {}

      const Color eval(const Vector&) const;

      //const int grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
