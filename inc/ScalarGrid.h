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
        m_Grid->setGridClass(openvdb::GRID_LEVEL_SET);
        m_Grid->setName("PyfxFloatGrid");
      }

      const float eval(const Vector &p) const;
      void set(int i, int j, int k, const float &value);
      const float& get(int i, int j, int k) const;
      void stamp(const ScalarField *f);
  };
}

#endif
