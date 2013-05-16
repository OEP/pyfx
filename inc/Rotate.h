#ifndef ROTATE_H_
#define ROTATE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class Rotate : public Volume<U,V>
  {
    private:
      const Vector m_Axis;
      const double m_Theta;
      const Volume<U,V> *m_Field;
      
      const Matrix
        m_Rotation,
        m_InvRotation,
        m_TRotation;

    protected:
      const U wrapEval(const U&) const;
      const V wrapGrad(const V&) const;
    public:
      Rotate(const Vector &axis, const double theta, const Volume<U,V> *field)
        : m_Axis(axis), m_Theta(theta), m_Field(field),
          m_Rotation(matrix::rotation(axis, theta)),
          m_InvRotation(m_Rotation.inverse()),
          m_TRotation(m_Rotation.transpose()) {}

      const U eval(const Vector &p) const
      {
        return wrapEval( m_Field->eval(p.rotate(m_Axis, -m_Theta)) );
      }

      const V grad(const Vector &p) const
      {
        return wrapGrad( m_Field->grad(p.rotate(m_Axis, -m_Theta)) );
      }

      const Box getBBox() const
      {
        return m_Field->getBBox().rotate(m_Axis, m_Theta);
      }
  };

  typedef Rotate<float, Vector> ScalarRotate;
  typedef Rotate<Vector,Matrix> VectorRotate;
  typedef Rotate<Matrix, int> MatrixRotate;
  typedef Rotate<Color, int> ColorRotate;

  // wrapEval()
  template <> const float ScalarRotate::wrapEval(const float &x) const 
  { return x; }

  template <> const Vector VectorRotate::wrapEval(const Vector &x) const 
  { return m_Rotation * x; }

  template <> const Matrix MatrixRotate::wrapEval(const Matrix &x) const 
  { return m_Rotation * x * m_TRotation; }

  template <> const Color ColorRotate::wrapEval(const Color &x) const 
  { return x; }

  // wrapGrad()
  template <> const Vector ScalarRotate::wrapGrad(const Vector &x) const 
  { return x; }

  template <> const Matrix VectorRotate::wrapGrad(const Matrix &x) const 
  { return m_Rotation * x; }

  template <> const int MatrixRotate::wrapGrad(const int &x) const 
  { return x; }

  template <> const int ColorRotate::wrapGrad(const int &x) const 
  { return x; }
}

#endif
