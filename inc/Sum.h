#ifndef PLUS_H_
#define PLUS_H_
#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Sum : public Volume<U, V>
  {
    private:
      const Volume<U,V> *m_A, *m_B;
    protected:
    public:
      Sum(const Volume<U,V> *a, const Volume <U,V> *b)
        : m_A(a), m_B(b) {}

      const U eval(const Vector& p) const
      {
        return m_A->eval(p) + m_B->eval(p);
      }

      const V grad(const Vector& p) const
      {
        return m_A->grad(p) + m_B->grad(p);
      }

      const Box getBBox() const
      {
        return m_A->getBBox().expand(m_B->getBBox());
      }
  };
}

#endif
