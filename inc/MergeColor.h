#ifndef MERGECOLOR_H_
#define MERGECOLOR_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class MergeColor : public ColorField
  {
    private:
      const ColorField *m_A, *m_B;
      const int m_Mode;
    protected:
    public:
      MergeColor(const ColorField *a, const ColorField *b, int mode=MergeColor::MAX)
        : m_A(a), m_B(b), m_Mode(mode) {}

      static const int
        PLUS,
        MINUS,
        MAX,
        MIN;

      const Color eval(const Vector&) const;
      const int grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
