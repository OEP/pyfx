#ifndef SPARSEGRID_H_
#define SPARSEGRID_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "macros.h"
#include "VolumeGrid.h"
#include "DenseGrid.h"

namespace vr
{
  template <typename U, typename V>
  class SparseGrid : public VolumeGrid<U,V>
  {
    using VolumeGrid<U,V>::size;
    using VolumeGrid<U,V>::defaultValue;
    using VolumeGrid<U,V>::oob;
    using VolumeGrid<U,V>::index;
    using VolumeGrid<U,V>::NX;
    using VolumeGrid<U,V>::NY;
    using VolumeGrid<U,V>::NZ;
    using VolumeGrid<U,V>::evalP;
    private:
    protected:
      const int m_PartitionSize;

      int m_PartitionShape[3];
      DenseGrid<U,V> **m_Values;

      const int partitionIndex(int i, int j, int k) const
      {
        const int
          pi = i / partitionSize(),
          pj = j / partitionSize(),
          pk = k / partitionSize();
        return pi + PNX() * (pj + PNY() * pk);
      }
      
      void init()
      {
        m_PartitionShape[0] = (int) ceil(NX() / (double) m_PartitionSize);
        m_PartitionShape[1] = (int) ceil(NY() / (double) m_PartitionSize);
        m_PartitionShape[2] = (int) ceil(NZ() / (double) m_PartitionSize);

        m_Values = new DenseGrid<U,V>*[partitions()];

        DenseGrid<U,V> **p = m_Values;
        for(int i = 0; i < partitions(); i++)
        {
          *p = NULL;
          p++;
        }
      }

    public:
      SparseGrid(Griddable *b, const Vector resolution,
        const U &dv, int partitionSize)
        : VolumeGrid<U,V>(b,resolution,dv),
          m_PartitionSize(partitionSize)
      {
        init();
      }
      
      SparseGrid(Griddable *b, int nx, int ny, int nz,
        const U &dv, int partitionSize)
        : VolumeGrid<U,V>(b,b->computeResolution(nx, ny, nz), dv),
          m_PartitionSize(partitionSize)
      {
        init();
      }

      ~SparseGrid()
      {
        DenseGrid<U,V> **p = m_Values;
        for(int i = 0; i < partitions(); i++)
        {
          if(*p != NULL)
          {
            (*p)->deleteGriddable();
            delete *p;
            *p = NULL;
          }
        }
        delete [] m_Values;
      }

      const U evalGrid(int i, int j, int k) const
      {
        if(oob(i,j,k))
          return defaultValue();

        const int
          pindex = partitionIndex(i,j,k),
          psize = partitionSize();
        DenseGrid<U,V> **p = m_Values + pindex;

        if(*p == NULL)
        {
          return defaultValue();
        }

        return (*p)->evalGrid(i%psize, j%psize, k%psize);
      }

      void set(int i, int j, int k, const U &c)
      {
        if(oob(i,j,k))
        {
          throw std::runtime_error("SparseGrid: Index out of bounds.");
        }

        if(c == defaultValue())
        {
          return;
        }

        const int
          pindex = partitionIndex(i,j,k),
          psize = partitionSize();
        DenseGrid<U,V> **p = m_Values + pindex;
        
        #pragma omp critical
        {
          if(*p == NULL)
          {
            Box *b = new Box(evalP(i,j,k), evalP(i+1,j+1,k+1));
            *p = new DenseGrid<U,V>(b, partitionSize(), defaultValue());
          }
        }

        (*p)->set(i%psize, j%psize, k%psize, c);
      }

      const int partitions() const { return PNX() * PNY() * PNZ(); }
      const int PNX() const { return m_PartitionShape[0]; }
      const int PNY() const { return m_PartitionShape[1]; }
      const int PNZ() const { return m_PartitionShape[2]; }
      const int partitionSize() const { return m_PartitionSize; }
  };

  typedef SparseGrid<float, Vector> ScalarSparseGrid;
  typedef SparseGrid<Vector, Matrix> VectorSparseGrid;
  typedef SparseGrid<Matrix, int> MatrixSparseGrid;
  typedef SparseGrid<Color, int> ColorSparseGrid;
}

#endif
