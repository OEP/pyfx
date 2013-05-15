#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Vector.h"
#include "Ray.h"

namespace vr
{
  class Triangle
  {
    private:
      Vector m_Points[3];
      Vector m_Edge[2];

      Vector m_Normal;
      double m_D;
      double
        m_InvDenom,
        m_Dot00,
        m_Dot01,
        m_Dot11;

      void update();
    protected:
    public:
      Triangle(const Vector&, const Vector&, const Vector&);
      Triangle(const Triangle&);

      const Vector& point(const int i) const;
      const double minimumDistance(const Vector&) const;
      const double minimumVertexDistance(const Vector&) const;
      const double intersects(const Ray &r) const;
      const bool intersects(const Ray &r, double &t) const;
      const bool contains(const Vector &p) const;

      const Vector& normal() const;
  };
}

#endif
