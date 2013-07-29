#ifndef IMPLICITVOLUME_H_
#define IMPLICITVOLUME_H_

#include <stdexcept>
#include <memory>

#include "Vector.h"
#include "Matrix.h"
#include "Color.h"
#include "Box.h"
#include "Vector.h"
#include "macros.h"

namespace vr
{
  template <typename U, typename V>
  class Volume
  {
    private:
    protected:
    public:
      virtual ~Volume() { }
      typedef std::shared_ptr< Volume<U, V> > Ptr;

      virtual const U eval(const Vector&) const
      {
        throw std::runtime_error("Illegal call to Volume::eval()");
      }

      virtual const V grad(const Vector&) const
      {
        throw std::runtime_error("Illegal call to Volume::grad()");
      }

      virtual const Box getBBox() const
      {
        throw std::runtime_error("Illegal call to Volume::getBBox()");
      }
  };

  typedef Volume<float, vr::Vector> ScalarField;
  typedef Volume<vr::Vector, vr::Matrix> VectorField;
  typedef Volume<vr::Matrix, int> MatrixField;
  typedef Volume<vr::Color, int> ColorField;
}

#endif
