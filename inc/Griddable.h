#ifndef GRIDDABLE_H_
#define GRIDDABLE_H_

#include <stdexcept>
#include <map>

#include "Vector.h"

namespace vr
{
  class Box;
  class Griddable
  {
    private:
    protected:
    public:
      virtual ~Griddable() {};
      virtual const Vector gridSpace(const Vector &p) const = 0;
      virtual const Vector worldSpace(const Vector &p) const = 0;
      virtual void gridSize(const Vector &res, int *dims) const = 0;
      virtual const Box getBBox() const = 0;
      virtual const Vector computeResolution(int n) const = 0;
      virtual void getProperties(std::map<std::string, std::string> &map) const = 0;
      virtual const std::string getShape() const = 0;
      
      virtual const Ray getAARay(const int i, const Vector &gs) const
      {
        const Vector origin = worldSpace(gs.replace(i, 0));
        return Ray(origin, (worldSpace(gs.replace(i, 1)) - origin));
      }
  };
}

#endif
