#ifndef VDBUTIL_H_
#define VDBUTIL_H_

#include <openvdb/openvdb.h>
#include "Vector.h"
#include "Color.h"

namespace vr
{
  const Vector to_pyfx(const openvdb::Vec3d&);
  const Color to_pyfx(const openvdb::Vec4d&);

  //! Convert Pyfx Vector to openvdb::Vec3d
  const openvdb::Vec3d to_openvdb(const Vector&);

  //! Convert a Pyfx Vector to openvdb::Vec4d
  const openvdb::Vec4d to_openvdb(const Color&);

  template <class GridType>
  const typename GridType::ValueType eval_trilinear(const GridType &grid, const Vector& p)
  {
    typedef typename GridType::ValueType ValueType;
    typedef typename GridType::ConstAccessor ConstAccessorType;
    ConstAccessorType accessor = grid.getAccessor();
    ValueType sum(0);
    openvdb::Coord dijk;

    const openvdb::Vec3d
      idx = grid.transformPtr()->worldToIndex(to_openvdb(p));
    const openvdb::Coord ijk = openvdb::Coord((int) idx.x(), (int) idx.y(), (int) idx.z());
    const openvdb::Vec3d wt[] = {
      openvdb::Vec3d(1) - (idx - ijk),
      idx - ijk,
    };

    for(int i = 0; i <= 1; i++)
    for(int j = 0; j <= 1; j++)
    for(int k = 0; k <= 1; k++)
    {
      dijk = ijk + openvdb::Coord(i,j,k);
      sum += wt[i][0] * wt[j][1] * wt[k][2] * accessor.getValue(dijk);
    }
    return sum;
  }
}

#endif
