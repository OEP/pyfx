#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "Camera.h"
#include "Image.h"
#include "Volume.h"
#include "DeepShadowMap.h"
#include "Light.h"

namespace vr
{
  class Scene
  {
    private:
      const Camera m_Camera;

      ColorField *m_Volume;
      ScalarField *m_Density;
      bool m_UseLights;

      const double m_Kappa;

      const Color shade(const Ray &r, const double step) const;

      std::vector<DeepShadowMap> m_DSMs;

    protected:
    public:
      Scene(const Camera& c, ScalarField *density, ColorField *volume=NULL, double kappa = 1.0, int dsmSize=100)
        : m_Camera(c), m_Volume(volume), m_Density(density),
          m_UseLights(true), m_Kappa(kappa) {};

      void addDSM(DeepShadowMap &dsm);
      void render(Image&, int, double) const;
      void render(Image&, int, double, size_t imw, size_t imh, size_t x0=0, size_t y0=0) const;
      const Color evalColor(const Vector &x) const;
      const Color evalMaterialColor(const Vector &x) const;
      const float evalDSM(const int i, const Vector &x) const;

      const Camera& camera() const;
      const ScalarField* density() const;
      const ColorField* color() const;
      const Box densityBox() const;
      const Box colorBox() const;
      const double kappa() const;

      void setUseLights(bool);
      void setDensity(ScalarField *density);
  };
}

#endif
