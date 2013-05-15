#ifndef RAY_H_
#define RAY_H_

#include <ostream>

#include "Vector.h"
#include "Vector.h"

namespace vr
{
  class Ray
  {
    private:
      Vector m_Origin;
      Vector m_Direction;

      Vector m_InvD;
      int m_Sign[3];
    protected:
    public:
      Ray() { update(); }
      Ray(const Ray &other);
      Ray(const Vector &p, const Vector &direction);
      
      const Vector& origin() const;
      const Vector& direction() const;
      const Vector& inverseDirection() const;
      int sign(const int) const;
      const Vector trace(const double) const;
      void update();
  };

  std::ostream& operator<<(std::ostream& out, const Ray &r);
}

#endif
