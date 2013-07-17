#ifndef SCALARGRID_H_
#define SCALARGRID_H_
#include "VolumeGrid.h"
namespace vr
{
  class ScalarGrid : public ScalarGridBase
  {
    private:
    protected:
      openvdb::FloatGrid::Ptr m_Grid;

      openvdb::GridBase::Ptr getGrid() const;
    public:
      ScalarGrid(const Box &b, const Vector resolution, const float &defaultValue)
      {
        m_Grid = openvdb::FloatGrid::create(defaultValue);
        setTransform(b, resolution);
      }

      const float eval(const Vector &p) const;
      void set(int i, int j, int k, const float &value);
  };
}

#endif
