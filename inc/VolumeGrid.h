#ifndef VOLUMEGRID_H_
#define VOLUMEGRID_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  template <typename U, typename V>
  class VolumeGrid : public Volume<U,V>
  {
    private:
    protected:
      Griddable *m_Griddable;
      const Vector m_Resolution;
      U m_Default;
      int m_Shape[3];

      int index(int i, int j, int k) const
      {
        return i + NX() * (j + NY() * k);
      }

    public:
      VolumeGrid(Griddable *b, const Vector resolution, const U defaultValue)
        : m_Griddable(b), m_Resolution(resolution), m_Default(defaultValue)
      {
        m_Griddable->gridSize(m_Resolution, m_Shape);
      }

      const U eval(const Vector &p) const
      {
        if(!contains(p))
        {
          return m_Default;
        }

        const Vector pp = gridCoordinates(p);
        const int
          i = (int) pp[0],
          j = (int) pp[1],
          k = (int) pp[2];
        const float
          wx = (1 - (pp[0] - i)),
          wy = (1 - (pp[1] - j)),
          wz = (1 - (pp[2] - k));

        return
             wx  *    wy  *    wz  * evalGrid(i  ,j  ,k  ) +
          (1-wx) *    wy  *    wz  * evalGrid(i+1,j  ,k  ) +
             wx  * (1-wy) *    wz  * evalGrid(i  ,j+1,k  ) +
          (1-wx) * (1-wy) *    wz  * evalGrid(i+1,j+1,k  ) +
             wx  *    wy  * (1-wz) * evalGrid(i  ,j  ,k+1) +
          (1-wx) *    wy  * (1-wz) * evalGrid(i+1,j  ,k+1) +
             wx  * (1-wy) * (1-wz) * evalGrid(i  ,j+1,k+1) +
          (1-wx) * (1-wy) * (1-wz) * evalGrid(i+1,j+1,k+1);
      }

      virtual const bool
      contains(const Vector &p) const
      {
        const Vector pp = m_Griddable->gridSpace(p);
        return
          pp[0] >= 0 && pp[0] <= 1 &&
          pp[1] >= 0 && pp[1] <= 1 &&
          pp[2] >= 0 && pp[2] <= 1;
      }

      virtual const Vector
      gridCoordinates(const Vector &p) const
      {
        const Vector v = m_Griddable->gridSpace(p);
        return Vector(
          v.x() * NX(),
          v.y() * NY(),
          v.z() * NZ());
      }

      const Box getBBox() const
      {
        return m_Griddable->getBBox();
      }

      void deleteGriddable()
      {
        if(m_Griddable != NULL)
        {
          delete m_Griddable;
          m_Griddable = NULL;
        }
      }

      const Vector evalP(int i, int j, int k) const
      {
        return m_Griddable->worldSpace(normalize(i,j,k));
      }

      const Vector normalize(int i, int j, int k) const
      {
        return Vector(
          i / (double) NX(),
          j / (double) NY(),
          k / (double) NZ());
      }

      void stamp(const Volume<U,V> *v)
      {
        #pragma omp parallel for
        for(int k = 0; k < NZ(); k++)
        {
          for(int j = 0; j < NY(); j++)
          {
            for(int i = 0; i < NX(); i++)
            {
              const Vector p = evalP(i,j,k);
              const U f = v->eval(p);
              set(i,j,k,f);
            }
          }
        }
      }

      void splat(const Vector &p, const U &value)
      {
        const Vector pp = gridCoordinates(p);
        const int
          i = (int) pp[0],
          j = (int) pp[1],
          k = (int) pp[2];
        const float
          wx = 1 - (pp[0] - i),
          wy = 1 - (pp[1] - j),
          wz = 1 - (pp[2] - k);

        add(i  , j  , k  ,    wx  *    wy  *    wz  * value);
        add(i+1, j  , k  , (1-wx) *    wy  *    wz  * value);
        add(i  , j+1, k  ,    wx  * (1-wy) *    wz  * value);
        add(i+1, j+1, k  , (1-wx) * (1-wy) *    wz  * value);
        add(i  , j  , k+1,    wx  *    wy  * (1-wz) * value);
        add(i+1, j  , k+1, (1-wx) *    wy  * (1-wz) * value);
        add(i  , j+1, k+1,    wx  * (1-wy) * (1-wz) * value);
        add(i+1, j+1, k+1, (1-wx) * (1-wy) * (1-wz) * value);
      }

      const Ray getAARay(int zero, int i, int j, int k)
      {
        return m_Griddable->getAARay( zero, normalize(i,j,k) );
      }

      const bool oob(int i, int j, int k) const
      {
        return
          i < 0 ||
          j < 0 ||
          k < 0 ||
          i >= NX() ||
          j >= NY() ||
          k >= NZ();
      }

      const int size() const { return NX() * NY() * NZ(); }
      const int NX() const { return m_Shape[0]; }
      const int NY() const { return m_Shape[1]; }
      const int NZ() const { return m_Shape[2]; }
      const U& defaultValue() const { return m_Default; }
      const Griddable* griddable() const { return m_Griddable; }

      void setDefaultValue(const U &defaultValue)
      {
        m_Default = defaultValue;
      }

      const Vector& resolution() const
      {
        return m_Resolution;
      }

      virtual void set(int i, int j, int k, const U &c)
      {
        throw std::runtime_error("VolumeGrid::set() not implemented");
      }

      const V grad(const Vector &p) const
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

        return
             wx  *    wy  *    wz  * gradGrid(i  ,j  ,k  ) +
          (1-wx) *    wy  *    wz  * gradGrid(i+1,j  ,k  ) +
             wx  * (1-wy) *    wz  * gradGrid(i  ,j+1,k  ) +
          (1-wx) * (1-wy) *    wz  * gradGrid(i+1,j+1,k  ) +
             wx  *    wy  * (1-wz) * gradGrid(i  ,j  ,k+1) +
          (1-wx) *    wy  * (1-wz) * gradGrid(i+1,j  ,k+1) +
             wx  * (1-wy) * (1-wz) * gradGrid(i  ,j+1,k+1) +
          (1-wx) * (1-wy) * (1-wz) * gradGrid(i+1,j+1,k+1);
      }
      
      const V gradGrid(int, int, int) const;

      virtual const U evalGrid(int i, int j, int k) const
      {
        throw
          std::runtime_error("VolumeGrid::evalGrid() unimplemented.");
      }

      virtual void add(int i, int j, int k, const U &c)
      {
        const U orig = evalGrid(i,j,k);
        set(i, j, k, orig + c);
      }
  };
  
  typedef VolumeGrid<float,Vector> ScalarVolumeGrid;
  typedef VolumeGrid<Vector,Matrix> VectorVolumeGrid;
  typedef VolumeGrid<Color,int> ColorVolumeGrid;
  typedef VolumeGrid<Matrix,int> MatrixVolumeGrid;
     

  template <>
  const Vector ScalarVolumeGrid::gradGrid(int, int, int) const;

}

#endif
