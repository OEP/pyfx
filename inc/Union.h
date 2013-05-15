#ifndef UNION_H_
#define UNION_H_

#include "Volume.h"

namespace vr
{
  class Union : public ScalarField
  {
    private:
      const ScalarField *m_VolumeA, *m_VolumeB;

    protected:
    public:
      Union(const ScalarField* a, const ScalarField* b)
        : m_VolumeA(a), m_VolumeB(b) { };

      const Box getBBox() const;
      const Vector grad(const Vector&) const;
      const float eval(const Vector&) const;
  };
}

#endif
