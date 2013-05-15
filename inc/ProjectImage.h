#ifndef PROJECTIMAGE_H_
#define PROJECTIMAGE_H_

#include "Volume.h"
#include "Vector.h"
#include "Matrix.h"
#include "Image.h"
#include "BaseProject2DPlane.h"

namespace vr
{
  class ProjectImage : public BaseProject2DPlane
  {
    private:
      const Image *m_Image;
      const int m_Channel;
    protected:
    public:
      ProjectImage(const Image *image,
            const Vector &view,
            const Vector &up,
            const Vector &x0 = Vector::ZEROS,
            const int channel = 0)
        : BaseProject2DPlane(view, up, x0),
          m_Image(image),
          m_Channel(channel)
        {}

      const float evalPlane(const Vector2D&) const;
  };
}

#endif
