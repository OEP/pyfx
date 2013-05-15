#ifndef VECTORCOMPONENT_H_
#define VECTORCOMPONENT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class VectorComponent : public ScalarField
  {
    private:
      const VectorField *m_V;
      const int m_Component;
    protected:
    public:
      VectorComponent(const VectorField *v, int c)
        : m_V(v), m_Component(c) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
