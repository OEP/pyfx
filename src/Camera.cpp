#include <cmath>
#include <cfloat>

#include <boost/lexical_cast.hpp>

#include "Camera.h"
#include "Ray.h"
#include "Box.h"
#include "macros.h"
using namespace vr;

Camera::Camera()
{
  m_Eye = Vector(0,0,0);
  m_Up = Vector(0, 1, 0);
  m_Right = Vector(1, 0, 0);
  m_View = m_Up ^ m_Right;
  m_FOV = 60.0 * M_PI / 180.0;
  m_AspectRatio = 1.77778;
  m_FarPlane = 100.0;
  m_NearPlane = 0.0;
  update();
}

Camera::Camera(const Vector &up, const Vector &right)
{
  m_Eye = Vector(0,0,0);
  m_Up = up.unit();
  m_Right = right.unit();
  m_View = m_Up ^ m_Right;
  m_FOV = 60.0 * M_PI / 180.0;
  m_AspectRatio = 1.77778;
  m_FarPlane = 100.0;
  m_NearPlane = 0.0;
  update();
}

Camera::Camera(const Camera &other)
{
  m_Eye = other.eye();
  m_Up = other.up();
  m_Right = other.right();
  m_View = other.view();
  m_FOV = other.horizontalFOV();
  m_AspectRatio = other.m_AspectRatio;
  m_FarPlane = other.m_FarPlane;
  m_NearPlane = other.m_NearPlane;
  update();
}

const Camera Camera::boundingFrustum(const Vector &x, const Box &b)
{
  const Vector mid = b.center(),
    view = mid - x,
    up = Vector(-view[1], view[0], view[2]);
  Camera c;
  c.look(x[0], x[1], x[2],
    mid[0], mid[1], mid[2],
    up[0], up[1], up[2]);

  double
    near = DBL_MAX,
    far = DBL_MIN,
    hfov = 0.0,
    vfov = 0.0;

  for(int i = 0; i < 8; i++)
  {
    const Vector p = b.corner(i) - x;
    const double mag = p.magnitude();
    const double
      xcp = p * c.right(),
      ycp = p * c.up(),
      zcp = p * c.view();

    far = std::max(far, mag);
    near = std::min(near, mag);
    hfov = std::max(hfov, fabs(atan(xcp / zcp)));
    vfov = std::max(vfov, fabs(atan(ycp / zcp)));
  }

  // Could possibly have a closer near plane if closest point
  // is on the near face of the bounding box.
  const Ray r = Ray(c.eye(), c.getView(0.50, 0.50));
  double t0, t1;
  if(b.intersects(r, t0, t1))
  {
    near = std::min(near, t0);
  }

  c.setNearPlane(near);
  c.setFarPlane(far);
  c.setFOV(RAD2DEG(2*hfov));
  c.setAspectRatio(hfov / vfov);

  return c;
}

void Camera::look(double eyeX, double eyeY, double eyeZ,
  double sX, double sY, double sZ,
  double upX, double upY, double upZ)
{
  const Vector
    eye = Vector(eyeX, eyeY, eyeZ),
    view = Vector(sX - eyeX, sY - eyeY, sZ - eyeZ),
    up = Vector(upX, upY, upZ);
  setEyeViewUp(eye, view, up);
}

const Vector Camera::getView(const double x, const double y) const
{
  const double
    xx = (2.0 * x - 1.0) * m_HTanFOV,
    yy = (2.0 * y - 1.0) * m_VTanFOV;

  return (yy * up() + xx * right() + view()).unit();
}

const Ray Camera::getRay(const double x, const double y) const
{
  const Vector dir = getView(x,y);
  return Ray(eye() + dir * nearPlane(), dir);
}

const CameraRay Camera::getRay(const int k, const int width, const int height, const int spp) const
{
  const int
    pixel_id = k / spp,
    i = pixel_id % width,
    j = pixel_id / width;
  const double
    dx = 1 / (double) width,
    dy = 1 / (double) height,
    nx = i / (double) (width-1),
    ny = j / (double) (height-1),
    xmid = nx + dx / 2,
    ymid = ny + dy / 2;

  if(k % spp == 0)
  {
    return CameraRay(getRay(xmid, ymid), pixel_id); 
  }
  else
  {
    return CameraRay(getRay(nx + drand48() * dx, ny + drand48() * dy), pixel_id);
  }
}

void Camera::setEyeViewUp(const Vector &eye, const Vector &view, const Vector &up)
{
  m_Eye = eye;
  m_View = view.unit();
  m_Up = (up - (up*m_View) * m_View).unit();
  m_Right = (m_View ^ m_Up).unit();
}

void Camera::setFOV(const double fov)
{
  m_FOV = DEG2RAD(fov);
  update();
}

void Camera::setAspectRatio(const double aspectRatio)
{
  m_AspectRatio = aspectRatio;
}

void Camera::setFarPlane(const double farPlane)
{
  m_FarPlane = std::max(0.0, farPlane);
}

void Camera::setNearPlane(const double nearPlane)
{
  m_NearPlane = std::max(0.0, nearPlane);
}

double Camera::viewLength() const
{
  return m_FarPlane - m_NearPlane;
}

double Camera::farPlane() const
{
  return m_FarPlane;
}

double Camera::nearPlane() const
{
  return m_NearPlane;
}


double Camera::aspectRatio() const
{
  return m_AspectRatio;
}

double Camera::horizontalFOV() const
{
  return m_FOV;
}

double Camera::verticalFOV() const
{
  return m_FOV / m_AspectRatio;
}

const Vector& Camera::eye() const
{
  return m_Eye;
}

const Vector& Camera::view() const
{
  return m_View;
}

const Vector& Camera::up() const
{
  return m_Up;
}

const Vector& Camera::right() const
{
  return m_Right;
}

const Box Camera::getBBox() const
{
  const Vector p0 = corner(0), p1 = corner(1);
  Box b = Box(p0.componentMin(p1), p0.componentMax(p1));
  for(int i = 2; i < 8; i++)
  {
    b = b.expand(corner(i));
  }
  return b;
}

const Vector Camera::corner(const int i) const
{
  const int
    zPos = (i & 4) != 0,
    yPos = (i & 2) != 0,
    xPos = (i & 1) != 0;
  return worldSpace(Vector(xPos, yPos, zPos));
}

const Vector Camera::gridSpace(const Vector &p) const
{
  const Vector
    pp = (p - eye()),
    x = (pp / (view() * pp)) - view();
  const double
    xcp = x * right(),
    ycp = x * up();
  return Vector(
    ((xcp/m_HTanFOV) + 1) / 2,
    ((ycp/m_VTanFOV) + 1) / 2,
    ((pp.magnitude() - nearPlane()) / viewLength()));
}

const Vector
Camera::worldSpace(const Vector &pg) const
{
  const double
    z = pg[2] * viewLength();
  const Ray
    r = getRay(pg[0], pg[1]);
  return r.trace(z);
}

void Camera::gridSize(const Vector &res, int *dims) const
{
  const Vector gridDims =
    Vector(horizontalFOV(), verticalFOV(), viewLength())  / res;
  dims[0] = ceil(gridDims[0]);
  dims[1] = ceil(gridDims[1]);
  dims[2] = ceil(gridDims[2]);
}

const Vector Camera::computeResolution(int n) const
{
  return Vector(horizontalFOV()/n, verticalFOV()/n, viewLength()/n);
}

const Camera
Camera::rotate(const Vector &axis, double theta)
{
  const Vector
    naxis = axis.unit(),
    rView = view().rotate(naxis, theta),
    rUp = up().rotate(naxis, theta),
    rEye = eye().rotate(naxis, theta);

  Camera newCamera(*this);
  newCamera.setEyeViewUp(rEye, rView, rUp);
  return newCamera;
}

void Camera::update()
{
  m_HTanFOV = tan(horizontalFOV()/2);
  m_VTanFOV = m_HTanFOV / m_AspectRatio;
}

void Camera::getProperties(map<string, string> &prop) const
{
  prop["eye"] = m_Eye.toString();
  prop["up"] = m_Up.toString();
  prop["right"] = m_Right.toString();
  prop["view"] = m_View.toString();
  prop["hfov"] = horizontalFOV();
  prop["ar"] = boost::lexical_cast<string>( aspectRatio() );
  prop["nearplane"] = boost::lexical_cast<string>( nearPlane() );
  prop["farplane"] = boost::lexical_cast<string>( farPlane() );
}

const std::string Camera::getShape() const
{
  return std::string("frustum");
}
