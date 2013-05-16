#ifndef MATRIX_H_
#define MATRIX_H_

#include <openvdb/openvdb.h>
#include "Vector.h"

namespace vr
{
  typedef openvdb::Vec3d Vector;
  typedef openvdb::math::Mat3d Matrix;

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
