#ifndef REPLACEBOX_H_
#define REPLACEBOX_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Box.h"

namespace vr
{
  template <typename U, typename V>
  class ReplaceBox : public Volume<U,V>
  {
    private:
      const Volume<U,V> *m_V;
      const Box m_Box;
    protected:
    public:
      ReplaceBox(const Volume<U,V> *v, const Box &b)
        : m_V(v), m_Box(b) {}

      const U eval(const Vector &p) const
      {
        return m_V->eval(p);
      }

      const V grad(const Vector &p) const
      {
        return m_V->grad(p);
      }

      const Box getBBox() const
      {
        return m_Box;
      }
  };
}

#endif
