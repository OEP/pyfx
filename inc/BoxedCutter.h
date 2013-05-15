#ifndef BOXEDCUTTER_H_
#define BOXEDCUTTER_H_

#include "VolumeArray.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class BoxedCutter : public VolumeArray
  {
    private:
      Vector res;
      Box bBox;
      Vector nUnits;
    protected:
    public:
      BoxedCutter(float resolution, Box boundingBox)
        : res(resolution,resolution,resolution),
          bBox(boundingBox), nUnits(bBox.length() / res)
      {}

      BoxedCutter(Vector resolutions, Box boundingBox)
        : res(resolutions), bBox(boundingBox), nUnits(bBox.length() / res)
      {}

      const float eval(int i, const Vector &p) const;
      const Vector grad(int i, const Vector &p) const;
      const Box getBBox(int i) const;
  };
}

#endif
