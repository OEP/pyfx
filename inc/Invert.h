#ifndef INVERT_H_
#define INVERT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Invert : public Volume<U,V>
  {
    private:
    protected:
      const Volume<U,V> *m_F;
    public:
      Invert(const Volume<U,V> *f): m_F(f) {}

      const U eval(const Vector &p) const
      {
        return -m_F->eval(p);
      }

      const V grad(const Vector &p) const
      {
        return -m_F->grad(p);
      }

      const Box getBBox() const
      {
        return m_F->getBBox();
      }
  };
}

#endif
