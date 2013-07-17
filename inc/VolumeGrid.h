#ifndef VOLUMEGRID_H_
#define VOLUMEGRID_H_

#include <openvdb/openvdb.h>

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "macros.h"

namespace vr
{
  const Vector to_pyfx(const openvdb::Vec3d&);
  const Color to_pyfx(const openvdb::Vec4d&);

  const openvdb::Vec3d to_openvdb(const Vector&);
  const openvdb::Vec4d to_openvdb(const Color&);

  template <class GridType>
  const typename GridType::ValueType eval_trilinear(const GridType &grid, const Vector& p)
  {
    typedef typename GridType::ValueType ValueType;
    typedef typename GridType::ConstAccessor ConstAccessorType;
    ConstAccessorType accessor = grid.getAccessor();
    ValueType sum(0);
    openvdb::Coord dijk;

    const openvdb::Vec3d
      idx = grid.transformPtr()->worldToIndex(to_openvdb(p));
    const openvdb::Coord ijk = openvdb::Coord((int) idx.x(), (int) idx.y(), (int) idx.z());
    const openvdb::Vec3d wt[] = {
      openvdb::Vec3d(1) - (idx - ijk),
      idx - ijk,
    };

    for(int i = 0; i <= 1; i++)
    for(int j = 0; j <= 1; j++)
    for(int k = 0; k <= 1; k++)
    {
      dijk = ijk + openvdb::Coord(i,j,k);
      sum += wt[i][0] * wt[j][1] * wt[k][2] * accessor.getValue(dijk);
    }
    return sum;
  }

  template <typename U, typename V>
  class VolumeGrid : public Volume<U,V>
  {
    private:
    protected:
      int m_Shape[3];
      
      void setTransform(const Box &b, const Vector &res)
      {
        const Vector
          shape = b.length() / res;
        openvdb::math::Transform::Ptr trafo = openvdb::math::Transform::createLinearTransform(1);

        trafo->preTranslate(to_openvdb(b.llc()));
        trafo->preScale(to_openvdb(res));
        getGrid()->setTransform(trafo);

        m_Shape[0] = (int) ceil(shape[0]);
        m_Shape[1] = (int) ceil(shape[1]);
        m_Shape[2] = (int) ceil(shape[2]);
      }

      virtual openvdb::GridBase::Ptr getGrid() const = 0;

    public:
      const Vector evalP(int i, int j, int k) const
      {
        const openvdb::Coord ijk(i, j, k);
        return to_pyfx(getGrid()->transformPtr()->indexToWorld(ijk));
      }

      const int NX() const { return m_Shape[0]; }
      const int NY() const { return m_Shape[1]; }
      const int NZ() const { return m_Shape[2]; }
      
      virtual void set(int i, int j, int k, const U& value) = 0;
  };
  
  typedef VolumeGrid<float, Vector> ScalarGridBase;
}

#endif
