#ifndef STEINERPATCH_H_
#define STEINERPATCH_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"

namespace vr
{
  class SteinerPatch : public ScalarField
  {
    private:
    protected:
    public:
      SteinerPatch() {}

      const float eval(const Vector&) const;
      const Vector grad(const Vector&) const;
      const Box getBBox() const;
  };
}

#endif
