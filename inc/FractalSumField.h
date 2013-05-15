#ifndef FRACTALSUMFIELD_H_
#define FRACTALSUMFIELD_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "FractalSum.h"

namespace vr
{
  class FractalSumField : public ScalarField
  {
    private:
      const FractalSum *m_FS;
    protected:
    public:
      FractalSumField(const FractalSum *fs)
        : m_FS(fs) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
