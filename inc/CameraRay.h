#ifndef CAMERARAY_H_
#define CAMERARAY_H_

namespace vr
{
  class CameraRay : public Ray
  {
    private:
      int m_RayId;
    protected:
    public:
      CameraRay(const Ray&, int);

      int getRayId() const;
  };
}

#endif
