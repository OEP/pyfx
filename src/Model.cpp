#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cfloat>

#include "Model.h"

#include "macros.h"

using namespace vr;

void Model::addTriangle(const Triangle &t)
{
  m_Box = m_Box.expand(t);
  m_Triangles.push_front(t);
}

const int Model::triangleCount() const
{
  return m_Triangles.size();
}

const Box Model::getBBox() const
{
  return m_Box;
}

const double Model::minimumDistance(const Vector &p) const
{
  std::deque<Triangle>::const_iterator it = m_Triangles.begin();
  double minDist = DBL_MAX;

  while(it != m_Triangles.end())
  {
    minDist = std::min(minDist, (*it).minimumDistance(p));
    it++;
  }

  return minDist;
}

bool Model::intersects(const Ray &r, std::set<double> &points) const
{
  if(m_Triangles.size() == 0) return false;
  std::deque<Triangle>::const_iterator it = m_Triangles.begin();

  double tmp;

  while(it != m_Triangles.end())
  {
    const Triangle &t = *it;

    if(t.intersects(r, tmp))
    {
      points.insert(tmp);
    }

    it++;
  }

  return points.size() > 0;
}

Model* Model::load(const std::string &filename)
{
  return Model::loadWavefront(filename);
}

Model* Model::loadWavefront(const std::string &filename)
{
  Model *m = new Model();
  // Dumb loader of OBJ files.
  std::vector<Vector> vertices;
  std::vector<int*> faces;

  std::ifstream filestream;
  filestream.open(filename);
  std::string line_stream;

  while(std::getline(filestream, line_stream))
  {
    if(line_stream.find_first_not_of(' ') == std::string::npos)
    {
      continue;
    }

    std::stringstream str_stream(line_stream);
    std::string type_str;
    str_stream >> type_str;

    if(type_str == "v")
    {
      double x,y,z;
      str_stream >> x >> y >> z;
      vertices.push_back(Vector(x,y,z));
    }
    else if(type_str == "f")
    {
      int *face = new int[3];
      str_stream >> face[0] >> face[1] >> face[2];
      faces.push_back(face);
    }
    else if(type_str.size() > 0 && type_str[0] == '#')
    {
    }
    else if(std::string("gusemtl#vnvt").find(type_str) == std::string::npos)
    {
      // Get pissed.
      throw std::runtime_error("Unknown type_str: " + type_str);
    }
  }

  int *face;
  for(size_t i = 0; i < faces.size(); i++)
  {
    face = faces[i];
    m->addTriangle(Triangle(vertices[face[0]-1], vertices[face[1]-1], vertices[face[2]-1]));
  }

  for(size_t i = 0; i < faces.size(); i++)
  {
    delete faces[i];
  }

  return m;
}
