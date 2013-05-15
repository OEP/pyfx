#ifndef RAMP_H_
#define RAMP_H_

#include <algorithm>

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "macros.h"

namespace vr
{
  template <typename U, typename V>
  class Ramp : public Volume<U,V>
  {
    private:
      const Vector m_P1, m_P2;
      const Volume<U,V> *m_F1, *m_F2;
    protected:
    public:
      Ramp(const Vector p1, const Vector p2,
        const Volume<U,V> *f1, const Volume<U,V> *f2)
        : m_P1(p1), m_P2(p2), m_F1(f1), m_F2(f2) {}

      const double computePhase(const Vector &p) const
      {
        const Vector
          k = m_P2 - m_P1,
          t = p - m_P1;
        const double
          pt = k.dot(t) / k.length(),
          amt = std::max(0.0, std::min(1.0, pt / k.length()));
        return amt;
      }

      const U eval(const Vector &p) const
      {
        const double phase = computePhase(p);
        return m_F1->eval(p) * (1-phase) + m_F2->eval(p) * phase;
      }

      const V grad(const Vector &p) const
      {
        throw std::runtime_error("Ramp::grad() not implemented");
      }

      const Box getBBox() const
      {
        return m_F1->getBBox().expand(m_F2->getBBox());
      }
  };
}

#endif
