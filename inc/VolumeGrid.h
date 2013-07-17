#ifndef VOLUMEGRID_H_
#define VOLUMEGRID_H_

#include <openvdb/openvdb.h>

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "macros.h"
#include "vdbutil.h"

namespace vr
{
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

      const Box getBBox() const
      {
        const openvdb::GridBase::Ptr grid = getGrid();
        Box b = Box(0);

        for(int i = 0; i <= 1; i++)
        for(int j = 0; j <= 1; j++)
        for(int k = 0; k <= 1; k++)
        {
          const openvdb::Coord 
            corneridx = openvdb::Coord(i * NX(), j * NY(), k * NZ());
          const openvdb::Vec3d
            corner = grid->transformPtr()->indexToWorld(corneridx);
          b = b.expand(to_pyfx(corner));
        }
        return b;
      }

      const int NX() const { return m_Shape[0]; }
      const int NY() const { return m_Shape[1]; }
      const int NZ() const { return m_Shape[2]; }
      
      virtual void set(int i, int j, int k, const U& value) = 0;
      virtual const U& get(int i, int j, int k) const = 0;
      virtual void stamp(const Volume<U,V> *f) = 0;
  };
  
  typedef VolumeGrid<float, Vector> ScalarGridBase;
}

#endif
