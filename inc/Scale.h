#ifndef SCALE_H_
#define SCALE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Scale : public Volume<U,V>
  {
    private:
      const float m_Lambda;
      const Volume<U,V> *m_Field;
    protected:
      const float getCoefficient() const;
    public:
      Scale(const float lambda, const Volume<U,V> *field)
        : m_Lambda(lambda), m_Field(field) {}

      const U eval(const Vector &p) const
      {
        return getCoefficient() * m_Field->eval(p / m_Lambda);
      }

      const V grad(const Vector &p) const
      {
        return getCoefficient() * m_Field->grad(p / m_Lambda);
      }

      const Box getBBox() const
      {
        return m_Field->getBBox().scale(m_Lambda);
      }
  };

  typedef Scale<float, Vector> ScalarScale;
  typedef Scale<Vector,Matrix> VectorScale;
  typedef Scale<Matrix, int> MatrixScale;
  typedef Scale<Color, int> ColorScale;

  // Specialized methods for getCoefficient().
  template<>
  const float ScalarScale::getCoefficient() const
  {
    return 1.0f;
  }

  template<>
  const float VectorScale::getCoefficient() const
  {
    return m_Lambda;
  }

  template<>
  const float MatrixScale::getCoefficient() const
  {
    return m_Lambda * m_Lambda;
  }

  template<>
  const float ColorScale::getCoefficient() const
  {
    return 1.0f;
  }

}

#endif
