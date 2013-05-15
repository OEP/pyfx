#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Translate : public Volume<U,V>
  {
    private:
      const Vector m_Direction;
      const Volume<U,V> *m_Field;
    protected:
    public:
      Translate(const Vector &direction, const Volume<U,V> *field)
        : m_Direction(direction), m_Field(field) {}

      const U eval(const Vector &p) const
      {
        return m_Field->eval(p - m_Direction);
      }

      const V grad(const Vector &p) const
      {
        return m_Field->grad(p - m_Direction);
      }

      const Box getBBox() const
      {
        return m_Field->getBBox().translate(m_Direction);
      }
  };
}

#endif
