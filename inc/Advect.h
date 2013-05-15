#ifndef ADVECT_H_
#define ADVECT_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Advect : public Volume<U,V>
  {
    private:
      const Volume<U,V> *m_F;
      const VectorField *m_U;
      const double m_DT;
    protected:
    public:
      Advect(const Volume<U,V> *f, const VectorField *u, const double dt)
        : m_F(f), m_U(u), m_DT(dt) {}

      const U eval(const Vector &p) const
      {
        return m_F->eval(p - m_U->eval(p) * m_DT);
      }

      const V grad(const Vector &p) const
      {
        throw std::runtime_error("Advect::grad() not implemented");
      }

      const Box getBBox() const
      {
        return m_F->getBBox();
      }
  };
}

#endif
