#include <cmath>

#include "Scene.h"
#include "macros.h"

using namespace vr;
using namespace std;

void Scene::render(Image &im, int rpp, double step) const
{
  render(im, rpp, step, im.width(), im.height(), 0, 0);
}

void Scene::render(Image &im, int rpp, double step, size_t imw, size_t imh, size_t x0, size_t y0) const
{
  x0 = CLAMP(x0, 0ul, imw);
  y0 = CLAMP(y0, 0ul, imh);

  #pragma omp parallel for 
  for(int j = 0; j < im.height(); j++)
  {
    if((j + y0) >= imh)
    {
      continue;
    }

    const double viewy = (y0+j) / (float) (imh - 1);
    for(int i = 0; i < im.width(); i++)
    {
      if((i + x0) >= imw)
      {
        continue;
      }

      const double viewx = (x0+i) / (float) (imw - 1);
      const Ray r = m_Camera.getRay(viewx, viewy);
      // TODO: Perturb ray if doing multiple samples
      const Color c = shade(r, step);
      im.add(i, j, c);
    }
  }
}

void Scene::addDSM(DeepShadowMap &dsm)
{
  m_DSMs.push_back(dsm);
}

const Color Scene::shade(const Ray &r, const double step) const
{
  double t0, t1;
  const Box bounds = m_Density->getBBox();
  
  if(!bounds.intersects(r, t0, t1))
  {
    return Color(0.0, 0.0);
  }

  t1 = std::min(t1, m_Camera.farPlane());


  double current = t0;

  float transmissivity = 1;
  Color light = Color(0,0,0,0);

  
  while(current < t1 && transmissivity > 0)
  {
    const Vector x = r.trace(current);
    const float deltaT = exp(-m_Kappa * m_Density->eval(x) * step);
    light = light + evalColor(x) * transmissivity * (1.0 - deltaT);
    transmissivity *= deltaT;
    current += step;
  }

  return Color(light.red(), light.green(), light.blue(), 1 - transmissivity);
}

const Color Scene::evalMaterialColor(const Vector &x) const
{
  if(m_Volume != NULL)
  {
    return m_Volume->eval(x);
  }

  const float val = std::max(0.0f, m_Density->eval(x));
  return Color(val, val);
}

const float Scene::evalDSM(const int i, const Vector &x) const
{
  return m_DSMs[i].field->eval(x);
}

const Color Scene::evalColor(const Vector &x) const
{
  if(!m_UseLights || m_DSMs.size() == 0)
  {
    return evalMaterialColor(x);
  }

  // TL = exp( - (dsm_lookup(i,x)) * kappa )
  // CI = sum[lights](C_i^L T_i^L(i)

  Color c(0);

  for(size_t i = 0; i < m_DSMs.size(); i++)
  {
    const float
      lookup = evalDSM(i, x),
      tl = exp( -lookup * m_Kappa );
    c += m_DSMs[i].light.color() * tl;
  }

  return evalMaterialColor(x) * c;
}

const Camera& Scene::camera() const
{
  return m_Camera;
}

const ScalarField* Scene::density() const
{
  return m_Density;
}

const Box Scene::densityBox() const
{
  return m_Density->getBBox();
}

const Box Scene::colorBox() const
{
  return (m_Volume != NULL) ? m_Volume->getBBox() : m_Density->getBBox();
}

const ColorField* Scene::color() const
{
  return m_Volume;
}

const double Scene::kappa() const
{
  return m_Kappa;
}

void Scene::setDensity(ScalarField *density)
{
  m_Density = density;
}

void Scene::setUseLights(bool b)
{
  m_UseLights = b;
}
