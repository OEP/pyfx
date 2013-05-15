#ifndef VECTOR_H_
#define VECTOR_H_

#include <openvdb/math/Vec3.h>
#include <ostream>

#include "Matrix.h"

namespace vr
{
  typedef openvdb::OPENVDB_VERSION_NAME::math::Vec3<double> Vector;

  namespace vector
  {
    static const Vector
      UX, UY, UZ;

    const Vector replace(const Vector &in, const int i, const double v);
    const Vector rotate(const Vector &in, const Vector &axis, const double v);
    const Vector lerpRotate(const Vector &a, const Vector &b, const double q);
    const Vector componentProduct(const Vector &a, const Vector &b);
    const Vector componentProduct(const Vector &a, const Vector &b);
    const std::string toString(const Vector &v);

//    using openvdb::OPENVDB_VERSION_NAME::math::maxComponent<double>;

//    const Vector componentMin(const Vector &a, const Vector &b);
//    const Vector componentMax(const Vector &a, const Vector &b);
  }
}


#endif
