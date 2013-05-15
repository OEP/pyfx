#ifndef MODEL_H_
#define MODEL_H_

#include <deque>
#include <string>
#include <set>

#include "Triangle.h"
#include "Box.h"

namespace vr
{
  class Model
  {
    private:
      std::deque<Triangle> m_Triangles;
      
      Box m_Box;
    protected:
      void addTriangle(const Triangle &t);
    public:
      Model() : m_Box(Box(0)) {}

      const Box getBBox() const;
      bool intersects(const Ray &r, std::set<double> &) const;
      const double minimumDistance(const Vector&) const;
      const int triangleCount() const;

      static Model* load(const std::string &filename);
      static Model* loadWavefront(const std::string &filename);
  };
}

#endif
