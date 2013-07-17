#include "ScalarGrid.h"

using namespace vr;

void ScalarGrid::set(int i, int j, int k, const float &value)
{
  openvdb::Coord ijk(ijk);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  accessor.setValue(ijk, value);
}

const float ScalarGrid::eval(const Vector &p) const
{
  return vr::eval_trilinear(*m_Grid, p);
}

openvdb::GridBase::Ptr ScalarGrid::getGrid() const
{
  return m_Grid;
}
