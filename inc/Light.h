#ifndef LIGHT_H_
#define LIGHT_H_

#include "Vector.h"
#include "Color.h"

namespace vr
{
  class Light
  {
    private:
      Vector m_Position;
      Color m_Color;
      double m_Intensity;
    protected:
    public:

      Light(const Vector &pos, const Color &c=Color(1), const double i=1.0)
        : m_Position(pos), m_Color(c), m_Intensity(i) {}

      const Color color() const;
      const Vector& position() const;

  };
}

#endif
