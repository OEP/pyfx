#include "VolumeGrid.h"

namespace vr
{
  template <>
  const Vector ScalarVolumeGrid::gradGrid(int i, int j, int k) const
  {
    return Vector(
      evalGrid(i+1, j, k) - evalGrid(i-1, j, k),
      evalGrid(i, j+1, k) - evalGrid(i, j-1, k),
      evalGrid(i, j, k+1) - evalGrid(i, j, k-1));
  }

  template <>
  const Matrix VectorVolumeGrid::gradGrid(int i, int j, int k) const
  {
    throw std::runtime_error("VectorVolumeGrid::gradGrid() not implemented");
  }

  template <>
  const int ColorVolumeGrid::gradGrid(int i, int j, int k) const
  {
    throw std::runtime_error("ColorVolumeGrid::gradGrid() not implemented");
  }

  template <>
  const int MatrixVolumeGrid::gradGrid(int i, int j, int k) const
  {
    throw std::runtime_error("MatrixVolumeGrid::gradGrid() not implemented");
  }
}
