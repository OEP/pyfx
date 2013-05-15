#ifndef VOLUMEARRAY_H_
#define VOLUMEARRAY_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class VolumeArray
  {
    private:
    protected:
    public:
      VolumeArray(){}

      virtual const float eval(int, const Vector &) const = 0;
      virtual const Vector grad(int, const Vector &) const = 0;
      virtual const Box getBBox(int) const = 0;
  };
}

#endif
