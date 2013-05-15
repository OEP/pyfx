#ifndef SLICEMASK_H_
#define SLICEMASK_H_

#include "VolumeArray.h"
#include "Vector.h"
#include "Matrix.h"
#include "types.h"

namespace vr
{
  /**
   *  SliceMask takes a set of scalar fields and aids in evaluating the i-th
   *  possible fragment created by slicing using the input scalar fields as
   *  masks.
   *
   *  FIXME: Fix the type name of m_Volumes so that Swig will work and we
   *    don't have such awful type names everywhere (use ScalarVolumeSet
   *    in types.h)
   */
  class SliceMask : public VolumeArray
  {
    private:
      const std::vector< ScalarField* > *m_Volumes;
    protected:
    public:
      SliceMask(const std::vector< ScalarField* > *volumes)
        : m_Volumes(volumes) {}

      const float eval(int i, const Vector&) const;
      const Vector grad(int i, const Vector&) const;
      const Box getBBox(int i) const;

      const size_t slices() const;
  };
}

#endif
