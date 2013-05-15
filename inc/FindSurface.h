#ifndef FINDSURFACE_H_
#define FINDSURFACE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class FindSurface : public VectorField
  {
    private:
      const ScalarField *m_F;
      const float m_InitialStep,
        m_Tolerance;
      const int m_MaxIterations;
    protected:
      const Vector inward(const Vector &p) const;
    public:
      FindSurface(const ScalarField *f,
        const float initialStep = 0.1f,
        const float tolerance = 0.0001f,
        const int maxIterations = 2000)
        : m_F(f),
          m_InitialStep(initialStep),
          m_Tolerance(tolerance),
          m_MaxIterations(maxIterations) {}

      const Vector eval(const Vector&) const;
      const Matrix grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
