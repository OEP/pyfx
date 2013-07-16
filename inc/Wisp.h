#ifndef WISP_H_
#define WISP_H_
/*
#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "BaseWisp.h"
#include "WispParameters.h"

namespace vr
{
  class Wisp : public BaseWisp
  {
    private:
    protected:
      void addPoints();
      void addPoint();

      WispParameters m_Parameters;
    public:
      Wisp
      (
        const WispParameters &params = WispParameters(),
        const Vector &res = Vector(0.01, 0.01, 0.01),
        const int partitionSize = 16,
        Box *box = NULL
      )
        : BaseWisp(res, partitionSize, box)
      {
        setParameters(params);
      }

      const Vector nextPoint();
      
      void setParameters(const WispParameters &wp)
      {
        m_Parameters = wp;
        m_FSPN1.setParameters(m_Parameters.FSPN1);
        m_FSPN2.setParameters(m_Parameters.FSPN2);
      }

      void getParameters(WispParameters &wp)
      {
        wp = m_Parameters;
      }

      const Box calcBBox() const;
  };
}
*/

#endif
