#include "Camera.h"
#include "DeepShadowMap.h"
#include "DenseGrid.h"
#include "macros.h"

using namespace vr;

// Do all samples and accumulate in grid.
void vr::compute_dsm(
  ScalarVolumeGrid *grid,
  const ScalarField *dns,
  const Vector &pl,
  const double ds,
  const int samples)
{
  #pragma omp parallel for
  for(int k = 0; k < grid->NZ(); k++)
  for(int j = 0; j < grid->NY(); j++)
  for(int i = 0; i < grid->NX(); i++)
  {
    double s = 0.0, sum = 0.0, stop, value;
    bool result = true;
    Ray ray;
    Vector p, fromLight;

    // Check if we can skip this point.
    for(int ii = -1; result && ii <= 1; ii++)
    for(int jj = -1; result && jj <= 1; jj++)
    for(int kk = -1; result && kk <= 1; kk++)
    {
      result &= dns->eval(grid->evalP(i+ii, j+jj, k+kk)) <= 0.0f;
    }

    if(!result)
    {
      continue;
    }

    // Start at centroid of grid cell
    // p = (grid->evalP(i,j,k) + grid->evalP(i+1,j+1,k+1)) / 2;
    p = grid->evalP(i,j,k);
    fromLight = p - pl;
    ray = Ray(pl, fromLight);
    stop = fromLight.length();

    while(s < stop)
    {
      p = ray.trace(s);
      value = ds * dns->eval(p);
      sum += value;
      // TODO: Throw in multiple sampling here.
      s += ds;
    }

    grid->set(i, j, k, sum);
  }
}
