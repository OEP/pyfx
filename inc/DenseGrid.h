#ifndef DENSEGRID_H_
#define DENSEGRID_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "macros.h"
#include "VolumeGrid.h"

namespace vr
{
  template <typename U, typename V>
  class DenseGrid : public VolumeGrid<U,V>
  {
    private:
      U *m_Values;
    protected:

      void init()
      {
        m_Values = new U[size()];
        U *p = m_Values;
        for(int i = 0; i < size(); i++)
        {
          *p = defaultValue();
          p++;
        }
      }

    public:
      using VolumeGrid<U,V>::size;
      using VolumeGrid<U,V>::defaultValue;
      using VolumeGrid<U,V>::oob;
      using VolumeGrid<U,V>::index;

      DenseGrid(Griddable *b, const Vector resolution, const U &dv)
        : VolumeGrid<U,V>(b,resolution,dv)
      {
        init();
      }

      DenseGrid(Griddable *b, const int dim, const U &dv)
        : VolumeGrid<U,V>(b, b->computeResolution(dim), dv)
      {
        init();
      }

      virtual ~DenseGrid()
      {
        delete [] m_Values;
      }

      const U evalGrid(int i, int j, int k) const
      {
        if(oob(i,j,k))
          return defaultValue();
        return m_Values[index(i,j,k)];
      }

      void set(int i, int j, int k, const U &c)
      {
        if(oob(i,j,k))
        {
          throw std::runtime_error("DenseGrid: Index out of bounds.");
        }
        m_Values[index(i,j,k)] = c;
      }
  };

  typedef DenseGrid<float,Vector> ScalarDenseGrid;
}

#endif
