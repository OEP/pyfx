#include "ScalarGrid.h"
#include "vdbutil.h"

using namespace vr;

void ScalarGrid::set(int i, int j, int k, const float &value)
{
  openvdb::Coord ijk(i, j, k);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  accessor.setValue(ijk, value);
}

const float& ScalarGrid::get(int i, int j, int k) const
{
  openvdb::Coord ijk(i, j, k);
  return m_Grid->getAccessor().getValue(ijk);
}

const float ScalarGrid::eval(const Vector &p) const
{
  return vr::eval_trilinear(*m_Grid, p);
}

openvdb::GridBase::Ptr ScalarGrid::getGrid() const
{
  return m_Grid;
}
