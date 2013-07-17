#include "VolumeGrid.h"

namespace vr
{
  const Vector to_pyfx(const openvdb::Vec3d& v)
  {
    return Vector(v[0], v[1], v[2]);
  }

  const Color to_pyfx(const openvdb::Vec4d& v)
  {
    return Color(v[0], v[1], v[2], v[3]);
  }

  const openvdb::Vec3d to_openvdb(const Vector &v)
  {
    return openvdb::Vec3d(v[0], v[1], v[2]);
  }

  const openvdb::Vec4d to_openvdb(const Color &v)
  {
    return openvdb::Vec4d(v[0], v[1], v[2], v[3]);
  }
}
