#ifndef BASEWISP_H_
#define BASEWISP_H_
/*

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "UniformPRN.h"
#include "SparseGrid.h"
#include "WispParameters.h"
#include "PerlinNoise.h"
#include "FractalSum.h"

namespace vr
{
  class BaseWisp : public ScalarField
  {
    private:
    protected:
      PerlinNoise m_PN;
      FractalSum m_FSPN1;
      FractalSum m_FSPN2;

      const Vector m_Resolution;
      const int m_PartitionSize;
 
      Box *m_Box, *m_GarbageBox;
      UniformPRN m_PRN;

      ScalarSparseGrid *m_Grid;

      virtual void addPoints() = 0;
    public:
      BaseWisp
      (
        const Vector &res = Vector(0.01, 0.01, 0.01),
        const int partitionSize = 16,
        Box *box = NULL
      )
      :
        m_Resolution(res),
        m_PartitionSize(partitionSize),
        m_Box(box),
        m_Grid(NULL),
        m_GarbageBox(NULL)
      {
        m_FSPN1 = FractalSum(&m_PN);
        m_FSPN2 = FractalSum(&m_PN);
      }

      virtual ~BaseWisp()
      {
        if(m_GarbageBox) delete m_GarbageBox;
        if(m_Grid) delete m_Grid;
      }

      const float eval(const Vector &p) const;
      const Vector grad(const Vector &p) const;
      const Box getBBox() const;
      virtual const Box calcBBox() const = 0;

      void initialize();
      void setSeed(const unsigned long seed);
  };
}
*/

#endif
