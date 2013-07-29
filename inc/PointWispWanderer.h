#ifndef POINTWISPWANDERER_H_
#define POINTWISPWANDERER_H_

#include "Vector.h"
#include "WispParameters.h"
#include "UniformPRN.h"

namespace vr
{
  class PointWispWanderer
  {
    private:
      UniformPRN m_PRN;
      WispParameters m_Parameters;
    protected:
    public:
      PointWispWanderer(const WispParameters p = WispParameters()):
        m_Parameters(p) {}

      const Vector nextPoint();
      void setParameters(const WispParameters &p);
      void setSeed(const unsigned long);
  };
}

#endif
