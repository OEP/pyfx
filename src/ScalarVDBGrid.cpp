#include "ScalarVDBGrid.h"
#include "macros.h"

using namespace vr;

const float ScalarVDBGrid::evalGrid(int i, int j, int k) const
{
  openvdb::Coord coord(i,j,k);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  return accessor.getValue(coord);
}

void ScalarVDBGrid::set(int i, int j, int k, const float &value) 
{
  openvdb::Coord coord(i,j,k);
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  accessor.setValue(coord, value);
}

void ScalarVDBGrid::stamp(const ScalarField *f)
{
  #pragma omp parallel for
  for(int k = 0; k < NZ(); k++)
  {
    openvdb::Coord coord;
    openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();

    for(int j = 0; j < NY(); j++)
    {
      for(int i = 0; i < NX(); i++)
      {
        coord = openvdb::Coord(i,j,k);
        const Vector p = evalP(i,j,k);
        const float value = f->eval(p);
        accessor.setValue(coord, value);
      }
    }
  }
}

const float ScalarVDBGrid::eval(const Vector &p) const
{
  const Vector pp = gridCoordinates(p);
  const int
    i = (int) pp[0],
    j = (int) pp[1],
    k = (int) pp[2];
  const float
    wx = (1 - (pp[0] - i)),
    wy = (1 - (pp[1] - j)),
    wz = (1 - (pp[2] - k));
  const float
    wwx[] = {wx, 1-wx},
    wwy[] = {wy, 1-wy},
    wwz[] = {wz, 1-wz};
  float sum = 0.0f;

  openvdb::Coord coord;
  openvdb::FloatGrid::Accessor accessor = m_Grid->getAccessor();
  for(int ii = i; ii <= i+1; ii++)
  for(int jj = j; jj <= j+1; jj++)
  for(int kk = k; kk <= k+1; kk++)
  {
    coord = openvdb::Coord(ii, jj, kk);
    sum += accessor.getValue(coord) * wwx[ii-i] * wwy[jj-j] * wwz[kk-k];
  }

  return sum;
}
