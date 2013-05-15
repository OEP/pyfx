#ifndef DIFFERENCE_H_
#define DIFFERENCE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Difference : public Volume<U,V>
  {
    private:
      const Volume<U,V> *m_V1, *m_V2;
    protected:
    public:
      Difference(const Volume <U,V> *v1, const Volume <U,V> *v2)
        : m_V1(v1), m_V2(v2) {}

      const U eval(const Vector &p) const
      {
        return m_V1->eval(p) - m_V2->eval(p);
      }

      const V grad(const Vector &p) const
      {
        return m_V1->grad(p) - m_V2->grad(p);
      }

      const Box getBBox() const
      {
        return m_V1->getBBox().expand(m_V2->getBBox());
      }
  };
}

#endif
