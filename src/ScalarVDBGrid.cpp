#include "ScalarVDBGrid.h"
#include "macros.h"

using namespace vr;

const float ScalarVDBGrid::evalGrid(int i, int j, int k) const
{
  openvdb::Coord coord(i,j,k);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  return accessor.getValue(coord);
}

void ScalarVDBGrid::set(int i, int j, int k, float value) 
{
  openvdb::Coord coord(i,j,k);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  accessor.setValue(coord, value);
}
