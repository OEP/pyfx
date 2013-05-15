#ifndef NATURALLOG_H_
#define NATURALLOG_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class NaturalLog : public ScalarField
  {
    private:
      const ScalarField *m_F1;
    protected:
    public:
      NaturalLog(const ScalarField *f1)
        : m_F1(f1) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
