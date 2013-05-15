#include "Ray.h"
#include "CameraRay.h"

using namespace vr;

CameraRay::CameraRay(const Ray& r, int id)
  : Ray(r)
{
  m_RayId = id;
}

int CameraRay::getRayId() const
{
  return m_RayId;
}
