#ifndef CAMERA_H_
#define CAMERA_H_

#include<map>
#include<string>

#include "Vector.h"
#include "Vector.h"
#include "Ray.h"
#include "CameraRay.h"
#include "Box.h"
#include "Griddable.h"

namespace vr
{
  class Camera : public Griddable
  {
    private:
      Vector m_Eye;
      Vector m_Up, m_Right, m_View;
      double m_FOV;
      double m_AspectRatio;

      double m_HTanFOV;
      double m_VTanFOV;

      double m_FarPlane;
      double m_NearPlane;

      void update();
    protected:
    public:
      virtual ~Camera() { };
      Camera();
      Camera(const Vector&, const Vector&);
      Camera(const Camera&);


      void look(double, double, double,
        double, double, double,
        double, double, double);

      static const Camera boundingFrustum(const Vector &x, const Box &b);

      void setEyeViewUp(const Vector&, const Vector&, const Vector&);
      void setAspectRatio(double);
      void setFOV(double);
      void setFarPlane(double);
      void setNearPlane(double);

      double horizontalFOV() const;
      double verticalFOV() const;
      double farPlane() const;
      double nearPlane() const;
      double viewLength() const;
      double aspectRatio() const;

      const Vector& eye() const;
      const Vector& view() const;
      const Vector& up() const;
      const Vector& right() const;
      const Ray getRay(const double, const double) const;
      const Vector getView(const double, const double) const;
      const CameraRay getRay(int i, int width, int height, int spp) const;

      const Camera rotate(const Vector&, double);
      const Vector corner(const int i) const;
     
      const Vector gridSpace(const Vector&) const;
      const Vector worldSpace(const Vector&) const;
      void gridSize(const Vector&, int*) const;
      const Box getBBox() const;
      const Vector computeResolution(int) const;

      void getProperties(std::map<std::string, std::string> &prop) const;
      const std::string getShape() const;
  };
}

#endif
