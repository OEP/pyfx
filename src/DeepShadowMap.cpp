#include "Camera.h"
#include "DeepShadowMap.h"
#include "DenseGrid.h"
#include "macros.h"

using namespace vr;

const float
  DeepShadowMap::DEFAULT_VALUE = 10.0f;

DeepShadowMap::~DeepShadowMap()
{
  for(size_t i = 0; i < m_Garbage.size(); i++)
  {
    delete m_Garbage[i];
  }
  for(size_t i = 0; i < m_Griddables.size(); i++)
  {
    delete m_Grids[i];
  }
}

const float
DeepShadowMap::eval(const int i, const Vector &x) const
{
  return m_Grids[i]->eval(x);
}

// Pick a random point inside voxel (i, j, k)
const Vector 
DeepShadowMap::pickRandom(UniformPRN &prn, const int i, const int j, const int k) const
{
  int ii, jj, kk;
  float r1, r2, r3;
  Vector gsp;

  const Vector base = (Vector(i,j,k) + Vector(i+1,j+1,k+1)) / (2*m_Resolution);
  int count = 0;

  // Loop til we draw a RN in the voxel.
  do
  {
    r1 = prn.eval();
    r2 = prn.eval();
    r3 = prn.eval();

    gsp = base +
      Vector(
        m_Jitter * (r1 - 0.5),
        m_Jitter * (r2 - 0.5),
        m_Jitter * (r3 - 0.5));

    ii = gsp[0] * m_Resolution,
    jj = gsp[1] * m_Resolution,
    kk = gsp[2] * m_Resolution;
    count++;
  } while(ii != i || jj != j || kk != k);

  return gsp;
}

//! Add a single light to the DSM.
const int
DeepShadowMap::add(const Vector &pl, const double ds, const ScalarField *dns, Camera *c)
{
  const Box b = dns->getBBox();
  // Allocate new camera only if we need to.
  if(c == NULL)
  {
    c = new Camera(Camera::boundingFrustum(pl, b));
    m_Garbage.push_back(c);

  }
  
  DBOUT("Bounding frustum: " << std::endl <<
    "Near: " << c->nearPlane() << std::endl <<
    "Far:  " << c->farPlane() << std::endl <<
    "HFOV: " << RAD2DEG(c->horizontalFOV()) << std::endl <<
    "VFOV: " << RAD2DEG(c->verticalFOV()) << std::endl <<
    "Eye:  " << c->eye() << std::endl <<
    "View: " << c->view() << std::endl <<
    "Up:   " << c->up() << std::endl <<
    "Rght: " << c->right()
  )

  m_Griddables.push_back(c);
  ScalarDenseGrid *g = new ScalarDenseGrid(c, m_Resolution, DeepShadowMap::DEFAULT_VALUE);
  m_Grids.push_back(g);

  #pragma omp parallel for
  for(int index = 0; index < size(); index++)
  {
    computeSamples(index, pl, m_Grids.size() - 1, ds, dns);
  }

  float sum = 0.0;
  int grid = m_Grids.size() - 1;
  for(int k = 0; k < m_Resolution; k++)
  for(int j = 0; j < m_Resolution; j++)
  for(int i = 0; i < m_Resolution; i++)
    sum += m_Grids[grid]->evalGrid(i,j,k);

  DBOUT("Average DSM: " << (sum / size()))

  return m_Grids.size() - 1;
}

// Do all samples and accumulate in grid.
void DeepShadowMap::computeSamples(
  const int index,
  const Vector &pl,
  const int grid,
  const double ds,
  const ScalarField *dns)
{
  float sum = 0.0;
  
  const int
    i = index % m_Resolution,
    j = (index / m_Resolution) % m_Resolution,
    k = (index / (m_Resolution*m_Resolution));

  float *samples = new float[m_Samples];
  for(int s = 0; s < m_Samples; s++)
  {
    samples[s] = computeRay(i, j, k, s, pl, grid, ds, dns);
  }

  for(int s = 0; s < m_Samples; s++)
  {
    sum += samples[s];
  }

  m_Grids[grid]->set(i, j, k, sum / m_Samples);

  delete[] samples;
}

const bool
DeepShadowMap::hasDensity(int i, int j, int k, int grid, const ScalarField *dns) const
{
  const Vector
    gsp = Vector(i,j,k) / m_Resolution,
    wsp = m_Griddables[grid]->worldSpace(gsp);
  return dns->eval(wsp) > 0;
}

const bool
DeepShadowMap::canSkip(int i, int j, int k, int grid, const ScalarField *dns) const
{
  bool result = true;
  for(int ii = -1; result && ii <= 1; ii++)
  for(int jj = -1; result && jj <= 1; jj++)
  for(int kk = -1; result && kk <= 1; kk++)
    result &= !hasDensity(i+ii,j+jj,k+kk,grid,dns);
  return result;
}

const float
DeepShadowMap::computeRay(
  const int i,
  const int j,
  const int k,
  const int sample,
  const Vector &pl,
  const int grid,
  const double ds,
  const ScalarField *dns)
{
  const Vector 
    centroid = (Vector(i,j,k) + Vector(i+1,j+1,k+1)) / (2*m_Resolution);

  Vector gsp = centroid;

  // Try to prevent MT from executing if we can help it.
  if(sample > 0)
  {
    #pragma omp critical
    {
      gsp = gsp + m_Jitter * m_PRN.nextVector(-0.5, 0.5);
    }
  }

  // Compute world space point, light-to-wsp vector.
  const Vector
    wsp = m_Griddables[grid]->worldSpace(gsp),
    fromLight = wsp - pl;

  const Ray ray = Ray(pl, fromLight);

  // No DSM needed if no density.
  if(dns->eval(wsp) <= 0 && canSkip(i,j,k,grid,dns))
  {
    return DeepShadowMap::DEFAULT_VALUE;
  }

  double s = m_Griddables[grid]->nearPlane();
  const double stop = fromLight.length();
  double value = 0.0;

  while(s < stop)
  {
    const Vector p = ray.trace(s);
    value += ds * dns->eval(p);
    s += ds;
  }

  return value;
}

const int DeepShadowMap::size() const
{
  return m_Resolution*m_Resolution*m_Resolution;
}

ScalarDenseGrid* DeepShadowMap::getMap(const int i) const
{
  return m_Grids[i];
}

void DeepShadowMap::setSeed(const unsigned long u)
{
  m_PRN.setSeed(u);
}

void DeepShadowMap::setSamples(int n)
{
  m_Samples = std::max(1, n);
}
