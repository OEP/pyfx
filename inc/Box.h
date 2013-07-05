#ifndef BOX_H_
#define BOX_H_

#include "Vector.h"
#include "Ray.h"
#include "Griddable.h"
#include "Triangle.h"

namespace vr
{
  class Griddable;
  class Box : public Griddable
  {
    private:
      Vector m_Bounds[2];
    protected:
    public:
      static const Box
        NIL,
        INFINITE;

      Box();
      Box(const Vector&, const Vector&);
      Box(const Box&);
      Box(const double);
      Box(const Vector&);

      bool contains(const Vector&) const;
      bool intersects(const Ray&) const;
      bool intersects(const Ray&, double&, double&) const;
      bool isNil() const;

      double sizeX() const;
      double sizeY() const;
      double sizeZ() const;

      const Vector gridSpace(const Vector&) const;
      const Vector worldSpace(const Vector&) const;
      void gridSize(const Vector&, int*) const;
      const Box getBBox() const;
      const Vector computeResolution(int,int,int) const;

      const Box expand(const Vector&) const;
      const Box expand(const Box&) const;
      const Box expand(const Triangle&) const;
      const Box pad(const Vector&) const;
      const Box intersect(const Box&) const;
      const Box translate(const Vector&) const;
      const Box rotate(const Vector&, const double) const;
      const Box scale(const double) const;
      const Box scale(const Vector&) const;
      const Vector corner(const int i) const;
      const Vector length() const;
     
      const Vector& llc() const;
      const Vector& urc() const;
      const Vector center() const;

      void getProperties(std::map<std::string, std::string> &prop) const;
      const std::string getShape() const;
  };
}
#endif
