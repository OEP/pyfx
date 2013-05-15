#ifndef SPLINEWISP_H_
#define SPLINEWISP_H_

#include "BaseWisp.h"
#include "WispParameters.h"
#include "WispChain.h"

namespace vr
{
  class SplineWisp : public BaseWisp
  {
    private:
      const WispChain *m_Chain;
    protected:
      void addSegmentPoints(size_t);
      void addPoints();
    public:
      SplineWisp
      (
        const WispChain *params,
        const Vector &res = Vector(0.01, 0.01, 0.01),
        const int partitionSize = 16,
        Box *box = NULL
      )
        : BaseWisp(res, partitionSize, box),
          m_Chain(params)
      {
      }

      const Box calcBBox() const;
      const Vector nextSegmentPoint(size_t);
      const Vector nextSegmentPoint(size_t, float&);
  };
}

#endif
