#ifndef SCALARVDBGRID_H_
#define SCALARVDBGRID_H_

#include "VolumeGrid.h"
#include <openvdb/openvdb.h>

namespace vr
{
  class ScalarVDBGrid : public ScalarVolumeGrid
  {
    private:
      openvdb::FloatGrid::Ptr m_Grid;
    protected:
    public:
      ScalarVDBGrid(Griddable *b, const Vector resolution, const float &dv)
        : ScalarVolumeGrid(b,resolution,dv)
      {
        openvdb::initialize();
        m_Grid = openvdb::FloatGrid::create(defaultValue());
      }

      const float evalGrid(int i, int j, int k) const;
      void set(int i, int j, int k, float v);
  };
}

#endif
