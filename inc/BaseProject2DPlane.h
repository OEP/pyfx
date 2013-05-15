#ifndef BASEPROJECT2DPLANE_H_
#define BASEPROJECT2DPLANE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Vector2D.h"

namespace vr
{
  class BaseProject2DPlane : public ScalarField
  {
    private:
      const Vector m_View, m_X0;
      Vector m_Up, m_Right;
    protected:
    public:
      BaseProject2DPlane(
          const Vector &view,
          const Vector &up,
          const Vector &x0 = Vector::ZEROS)
        : 
          m_View(view.unit()),
          m_X0(x0)
        {
          m_Right = (up ^ view).unit();
          m_Up = (m_View ^ m_Right).unit();
        }

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;

      virtual const float evalPlane(const Vector2D&) const = 0;
  };
}

#endif
