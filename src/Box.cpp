#include <algorithm>
#include <limits>
#include <cfloat>

#include "Box.h"
#include "Vector.h"
#include "Ray.h"
#include "Vector.h"
#include "macros.h"

using namespace vr;
using std::map;
using std::string;

using openvdb::OPENVDB_VERSION_NAME::math::maxComponent;
using openvdb::OPENVDB_VERSION_NAME::math::minComponent;

const Box
  Box::NIL = Box(-DBL_MAX),
  Box::INFINITE = Box(DBL_MAX);

Box::Box()
{
  m_Bounds[0] = Vector(-0.5, -0.5, -0.5);
  m_Bounds[1] = Vector(0.5, 0.5, 0.5);
}

Box::Box(const Vector& min, const Vector& max)
{
  m_Bounds[0] = min;
  m_Bounds[1] = max;
}

Box::Box(const Vector& v)
{
  m_Bounds[0] = -v;
  m_Bounds[1] =  v;
}

Box::Box(const double r)
{
  m_Bounds[0] = Vector(-r,-r,-r);
  m_Bounds[1] = Vector(r,r,r);
}

Box::Box(const Box &other)
{
  m_Bounds[0] = Vector(other.m_Bounds[0]);
  m_Bounds[1] = Vector(other.m_Bounds[1]);
}

double Box::sizeX() const
{
  return m_Bounds[1].x() - m_Bounds[0].x();
}

double Box::sizeY() const
{
  return m_Bounds[1].y() - m_Bounds[0].y();
}

double Box::sizeZ() const
{
  return m_Bounds[1].z() - m_Bounds[0].z();
}

bool Box::intersects(const Ray& r) const
{
  double dump1, dump2;
  return intersects(r, dump1, dump2);
}

bool Box::isNil() const
{
  return
    m_Bounds[0][0] > m_Bounds[1][0] ||
    m_Bounds[0][1] > m_Bounds[1][1] ||
    m_Bounds[0][2] > m_Bounds[1][2];
}

const Box Box::pad(const Vector &amt) const
{
  return Box(
    m_Bounds[0] - amt,
    m_Bounds[1] + amt);
}

const Box Box::expand(const Vector &p) const
{
  if(isNil()) return *this;
  return Box(
    minComponent(m_Bounds[0], p),
    maxComponent(m_Bounds[1], p));
}

const Box Box::expand(const Box &other) const
{
  if(other.isNil()) return *this;
  if(isNil()) return other;

  return
    expand(other.m_Bounds[0])
    .expand(other.m_Bounds[1]);
}

const Box Box::expand(const Triangle &other) const
{
  return 
      expand(other.point(0))
      .expand(other.point(1))
      .expand(other.point(2));
}

const Box Box::intersect(const Box &other) const
{
  if(other.isNil()) return *this;
  if(isNil()) return other;

  const Vector
    p0 = maxComponent(m_Bounds[0], other.m_Bounds[0]),
    p1 = minComponent(m_Bounds[1], other.m_Bounds[1]),
    pp0 = minComponent(p0, p1);
      
  return Box(pp0, p1);
}

const Vector Box::length() const
{
  return m_Bounds[1] - m_Bounds[0];
}

const Box Box::translate(const Vector &direction) const
{
  return Box(
    m_Bounds[0] + direction,
    m_Bounds[1] + direction);
}

const Box Box::scale(const double factor) const
{
  return Box(
    m_Bounds[0] * factor,
    m_Bounds[1] * factor);
}

const Box Box::scale(const Vector &scale) const
{
  using vector::componentProduct;
  return Box(
    componentProduct(m_Bounds[0], scale),
    componentProduct(m_Bounds[1], scale)
  );
}

const Box Box::rotate(const Vector &axis, const double theta) const
{
  using vector::rotate;

  const Vector
    p0 = rotate(m_Bounds[0], axis, theta),
    p1 = rotate(m_Bounds[1], axis, theta);

  return Box(minComponent(p0, p1), maxComponent(p0, p1));
}

bool Box::contains(const Vector &p) const
{
  return
    m_Bounds[0][0] <= p[0] &&
    m_Bounds[0][1] <= p[1] &&
    m_Bounds[0][2] <= p[2] &&
    m_Bounds[1][0] >= p[0] &&
    m_Bounds[1][1] >= p[1] &&
    m_Bounds[1][2] >= p[2];
}

const Vector& Box::llc() const
{
  return m_Bounds[0];
}

const Vector& Box::urc() const
{
  return m_Bounds[1];
}

const Vector Box::gridSpace(const Vector &p) const
{
  const Vector pp = p - llc();
  return Vector(
    pp[0] / sizeX(),
    pp[1] / sizeY(),
    pp[2] / sizeZ());
}

const Vector
Box::worldSpace(const Vector &pg) const
{
  return llc() + vector::componentProduct(pg, length());
}

void Box::gridSize(const Vector &res, int *dims) const
{
  const Vector gridDims = length() / res;
  dims[0] = ceil(gridDims[0]);
  dims[1] = ceil(gridDims[1]);
  dims[2] = ceil(gridDims[2]);
}

const Box Box::getBBox() const
{
  return *this;
}

const Vector Box::computeResolution(int n) const
{
  return length() / n;
}

const Vector Box::corner(const int i) const
{
  const int
    addX = (i & 1) != 0,
    addY = (i & 2) != 0,
    addZ = (i & 4) != 0;

  return m_Bounds[0] +
    addX * sizeX() * vector::UX +
    addY * sizeY() * vector::UY +
    addZ * sizeZ() * vector::UZ;
}


const Vector Box::center() const
{
  return (llc() + urc()) / 2;
}

bool Box::intersects(const Ray& r, double& t0, double& t1) const
{
  double tmin, tmax, tymin, tymax, tzmin, tzmax;

  tmin = (m_Bounds[r.sign(0)].x() - r.origin().x()) * r.inverseDirection().x();
  tmax = (m_Bounds[1-r.sign(0)].x() - r.origin().x()) * r.inverseDirection().x();

  tymin = (m_Bounds[r.sign(1)].y() - r.origin().y()) * r.inverseDirection().y();
  tymax = (m_Bounds[1-r.sign(1)].y() - r.origin().y()) * r.inverseDirection().y();
  
  if(tmin > tymax || tymin > tmax)
  {
    return false;
  }

  tmin = std::max(tmin, tymin);
  tmax = std::min(tmax, tymax);

  tzmin = (m_Bounds[r.sign(2)].z() - r.origin().z()) * r.inverseDirection().z();
  tzmax = (m_Bounds[1-r.sign(2)].z() - r.origin().z()) * r.inverseDirection().z();

  if(tmin > tzmax || tzmin > tmax)
  {
    return false;
  }

  tmin = std::max(tmin, tzmin);
  tmax = std::min(tmax, tzmax);

  t0 = std::max(tmin,  0.0);
  t1 = std::min(tmax, std::numeric_limits<double>::max());

  if(t1 < t0)
  {
    std::swap(t1, t0);
  }

  return true;
}

const std::string Box::getShape() const
{
  return std::string("box");
}


void Box::getProperties(map<string,string> &prop) const
{
  prop["llc"] = vector::toString(m_Bounds[0]);
  prop["urc"] = vector::toString(m_Bounds[1]);
}
