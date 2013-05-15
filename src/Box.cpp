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
  return m_Bounds[1].X() - m_Bounds[0].X();
}

double Box::sizeY() const
{
  return m_Bounds[1].Y() - m_Bounds[0].Y();
}

double Box::sizeZ() const
{
  return m_Bounds[1].Z() - m_Bounds[0].Z();
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
    m_Bounds[0].componentMin(p),
    m_Bounds[1].componentMax(p));
}

const Box Box::expand(const Box &other) const
{
  if(other.isNil()) return *this;
  if(isNil()) return other;
  return Box(
    m_Bounds[0].componentMin(other.m_Bounds[0]),
    m_Bounds[1].componentMax(other.m_Bounds[1]));
}

const Box Box::expand(const Triangle &other) const
{
  return Box(
    m_Bounds[0]
      .componentMin(other.point(0))
      .componentMin(other.point(1))
      .componentMin(other.point(2)),
    m_Bounds[1]
      .componentMax(other.point(0))
      .componentMax(other.point(1))
      .componentMax(other.point(2)));
}

const Box Box::intersect(const Box &other) const
{
  if(other.isNil()) return *this;
  if(isNil()) return other;
  const Vector
    p0 = m_Bounds[0].componentMax(other.m_Bounds[0]),
    p1 = m_Bounds[1].componentMin(other.m_Bounds[1]),
    pp0 = p0.componentMin(p1);
      
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
  return Box(
    m_Bounds[0].componentProduct(scale),
    m_Bounds[1].componentProduct(scale));
}

const Box Box::rotate(const Vector &axis, const double theta) const
{
  const double
    sx = sizeX(),
    sy = sizeY(),
    sz = sizeZ();
  const Vector
    p1 = m_Bounds[0].rotate(axis, theta),
    u = vector::UX.rotate(axis, theta),
    v = vector::UY.rotate(axis, theta),
    w = vector::UZ.rotate(axis, theta),
    dx = u * sx,
    dy = v * sy,
    dz = w * sz,
    p2 = p1 + dx,
    p3 = p1 + dy,
    p4 = p1 + dz,
    p5 = p1 + dx + dy,
    p6 = p1 + dy + dz,
    p7 = p1 + dx + dy,
    p8 = p1 + dx + dy + dz,
    pmin = 
      p1.componentMin(p2)
        .componentMin(p3)
        .componentMin(p4)
        .componentMin(p5)
        .componentMin(p6)
        .componentMin(p7)
        .componentMin(p8),
    pmax = 
      p1.componentMax(p2)
        .componentMax(p3)
        .componentMax(p4)
        .componentMax(p5)
        .componentMax(p6)
        .componentMax(p7)
        .componentMax(p8);
  return Box(pmin, pmax);
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
  return llc() + pg.componentProduct(length());
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

  tmin = (m_Bounds[r.sign(0)].X() - r.origin().X()) * r.inverseDirection().X();
  tmax = (m_Bounds[1-r.sign(0)].X() - r.origin().X()) * r.inverseDirection().X();

  tymin = (m_Bounds[r.sign(1)].Y() - r.origin().Y()) * r.inverseDirection().Y();
  tymax = (m_Bounds[1-r.sign(1)].Y() - r.origin().Y()) * r.inverseDirection().Y();
  
  if(tmin > tymax || tymin > tmax)
  {
    return false;
  }

  tmin = std::max(tmin, tymin);
  tmax = std::min(tmax, tymax);

  tzmin = (m_Bounds[r.sign(2)].Z() - r.origin().Z()) * r.inverseDirection().Z();
  tzmax = (m_Bounds[1-r.sign(2)].Z() - r.origin().Z()) * r.inverseDirection().Z();

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
  prop["llc"] = m_Bounds[0].toString();
  prop["urc"] = m_Bounds[1].toString();
}
