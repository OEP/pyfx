#ifndef VDBGRID_H_
#define VDBGRID_H_

#include "VolumeGrid.h"
#include <openvdb/Grid.h>

namespace vr
{
  template <typename U, typename V>
  class VDBGrid : public VolumeGrid<U,V>
  {
    private:
      typedef typename openvdb::tree::Tree4<U, 5, 4, 3>::Type   TreeType;
      typedef openvdb::Grid<TreeType>                  GridType;

      GridType m_Grid;
    protected:
    public:
      
      void set(int i, int j, int k, const U &c)
      {
        throw std::runtime_error("VolumeGrid::set() not implemented");
      }
      
      const U evalGrid(int i, int j, int k)
      {
        throw std::runtime_error("VolumeGrid::set() not implemented");
      }

  };
}

#endif
