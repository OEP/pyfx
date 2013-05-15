#ifndef MATRIX_H_
#define MATRIX_H_

#include <openvdb/math/Mat3.h>
#include <openvdb/math/Vec3.h>
#include "Vector.h"

namespace vr
{
  typedef openvdb::OPENVDB_VERSION_NAME::math::Mat3<double> Matrix;
  typedef openvdb::OPENVDB_VERSION_NAME::math::Vec3<double> Vector;

  namespace matrix
  {
    const Matrix
      EYE = Matrix(1, 0, 0,
                   0, 1, 0,
                   0, 0, 1);
    const Matrix
      PAULI[3] =
      {
        Matrix( 0, 0, 0,
                0, 0, 1,
                0,-1, 0 ),
        Matrix( 0, 0,-1,
                0, 0, 0,
                1, 0, 0 ),
        Matrix( 0, 1, 0,
               -1, 0, 0,
                0, 0, 0 )
      };

    const Matrix rotation(const Vector &axis, const double theta);
  }
}

#endif
