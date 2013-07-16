#ifndef LEVELSET_H_
#define LEVELSET_H_
/*
#include <set>

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Model.h"
#include "DenseGrid.h"
#include "VolumeGrid.h"

namespace vr
{
  class Levelset : public ScalarField
  {
    private:
      Model *m_Model;
      int m_Passes;
      double m_Threshold;
      ScalarDenseGrid *m_Grid;
      Box m_Box;
      std::set<int> m_Finalized;
    protected:
      const Ray getAARay(int,int,int) const;
      const bool isFinal(int,int,int,int) const;
      const bool isFinal(int,int,int) const;
      void finalize(int,int,int,int);
      void finalize(int,int,int);
      const int unique(int,int,int) const;
      const int NX(int) const;
      const int NY(int) const;
      const int NZ(int) const;
      const double sizeZ(int) const;
      const double getStep(int) const;
      const double evalGrid(int,int,int,int) const;
      void set(int,int,int,int,double);
      void performPass(int);
      void computeMinimumDistance();
      static void remap(int, int&, int&, int&);
    public:
      Levelset(Model *m, const Vector&, const Vector&, double thresh=1.0);
      ~Levelset();

      const int NX() const;
      const int NY() const;
      const int NZ() const;

      const ScalarDenseGrid* getGrid() const { return m_Grid; }
  
      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}
*/
#endif
