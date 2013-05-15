#ifndef GINSU_H_
#define GINSU_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Vector2D.h"
#include "types.h"
#include "BaseProject2DPlane.h"

namespace vr
{
  class Ginsu : public BaseProject2DPlane
  {
    private:
      const Vector2DSequence *m_Vectors;
    protected:
    public:
      // TODO: Get SWIG to work with VectorSequence in constructor.
      Ginsu(const std::vector<Vector2D> *vectors,
            const Vector &view,
            const Vector &up,
            const Vector &x0 = Vector::ZEROS)
        : BaseProject2DPlane(view, up, x0), m_Vectors(vectors)
        {}

      const float evalPlane(const Vector2D&) const;
      const int sign(const int, const Vector2D&) const; 
  };
}

#endif
