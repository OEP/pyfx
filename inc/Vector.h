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
  }
}


#endif
