#ifndef EXTRACTSLICE_H_
#define EXTRACTSLICE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "SliceMask.h"

namespace vr
{
  class ExtractSlice : public ScalarField
  {
    private:
      const VolumeArray *m_Mask;
      const int m_SliceIndex;
    protected:
    public:
      ExtractSlice(const VolumeArray *mask, int sliceIndex)
        : m_Mask(mask), m_SliceIndex(sliceIndex) {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
