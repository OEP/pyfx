#ifndef PASSTHROUGH_H_
#define PASSTHROUGH_H_

/**
 * Given any field f(x) and vector field v(x), return g(x) s.t.
 *
 *   g(x) = f( v(x) )
 */

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class PassThrough : public Volume<U,V>
  {
    private:
      const Volume<U,V> *m_F;
      const VectorField *m_V;
    protected:
    public:
      PassThrough(const Volume<U,V> *f, const VectorField *v) :
        m_F(f), m_V(v) {}

      const U eval(const Vector &p) const
      {
        return m_F->eval( m_V->eval(p) );
      }

      const V grad(const Vector &p) const
      {
        throw std::runtime_error("PassThrough::grad() not implemented");
      }

      const Box getBBox() const
      {
        return m_F->getBBox();
      }
  };
}

#endif
