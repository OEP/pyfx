#ifndef MATRIX_H_
#define MATRIX_H_

#include <openvdb/math/Mat3.h>
#include "Vector.h"

namespace vr
{
  typedef openvdb::OPENVDB_VERSION_NAME::math::Mat3<double> Matrix;
}

#endif
