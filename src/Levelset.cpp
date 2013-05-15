#include "Levelset.h"

#include <cfloat>
#include <cassert>

using namespace vr;

using std::min;

Levelset::Levelset(Model *m, const Vector &resolution, const Vector &padding, const double thresh)
  : m_Model(m), m_Passes(0), m_Threshold(thresh)
{
  m_Box = m_Model->getBBox().pad(padding);
  m_Grid = new ScalarDenseGrid(&m_Box, resolution, -1e9);

  DBOUT("Pass 0")
  performPass(0);
  DBOUT("Pass 1")
  performPass(1);
  DBOUT("Pass 2")
  performPass(2);
}

Levelset::~Levelset()
{
  delete m_Grid;
}

const int Levelset::NX() const { return m_Grid->NX(); } 
const int Levelset::NY() const { return m_Grid->NY(); } 
const int Levelset::NZ() const { return m_Grid->NZ(); } 

const float Levelset::eval(const Vector &p) const
{
  return m_Grid->eval(p);
}

const Vector Levelset::grad(const Vector &p) const
{
  return m_Grid->grad(p);
}

const Box Levelset::getBBox() const
{
  return m_Grid->getBBox();
}

const Ray Levelset::getAARay(int pass, int i, int j) const
{
  int k = 0;
  remap(pass, i, j, k);
  return m_Grid->getAARay(pass, i, j, k);
}

const int Levelset::NX(int pass) const
{
  switch(pass)
  {
    case 0: return m_Grid->NY();
    case 1:
    case 2: return m_Grid->NX();
  }
  throw std::runtime_error("Levelset::NX(): Invalid pass.");
}

const int Levelset::NY(int pass) const
{
  switch(pass)
  {
    case 0:
    case 1: return m_Grid->NZ();
    case 2: return m_Grid->NY();
  }
  throw std::runtime_error("Levelset::NY(): Invalid pass.");
}

const int Levelset::NZ(int pass) const
{
  switch(pass)
  {
    case 0: return m_Grid->NX();
    case 1: return m_Grid->NY();
    case 2: return m_Grid->NZ();
  }
  throw std::runtime_error("Levelset::NZ(): Invalid pass.");
}

const double Levelset::getStep(int pass) const
{
  return m_Grid->resolution()[pass];
}

void Levelset::remap(const int pass, int &i, int &j, int &k)
{
  const int
    ii = i,
    jj = j,
    kk = k;

  switch(pass)
  {
    case 0: i = kk; j = ii; k = jj; return;
    case 1: i = ii; j = kk; k = jj; return;
    case 2: return;
  }

  throw std::runtime_error("Levelset::remap(): invalid pass");
}

const double Levelset::evalGrid(int pass, int i, int j, int k) const
{
  remap(pass,i,j,k);
  return m_Grid->evalGrid(i,j,k);
}

void Levelset::set(int pass, int i, int j, int k, double value)
{
  remap(pass,i,j,k);
  m_Grid->set(i,j,k,value);
}

const bool Levelset::isFinal(int pass, int i, int j, int k) const
{
  remap(pass, i, j, k);
  return isFinal(i,j,k);
}

const bool Levelset::isFinal(int i, int j, int k) const
{
  const int uniq = unique(i,j,k);
  return m_Finalized.find(uniq) != m_Finalized.end();
}

void Levelset::finalize(int pass, int i, int j, int k)
{
  remap(pass, i, j, k);
  return finalize(i, j, k);
}

void Levelset::finalize(int i, int j, int k)
{
  m_Finalized.insert(unique(i,j,k));
}

const int Levelset::unique(int i, int j, int k) const
{
  return i + NY() * j + NY() * NX() * k;
}

const double Levelset::sizeZ(int pass) const
{
  return m_Grid->getBBox().length()[pass];
}

void Levelset::computeMinimumDistance()
{
  for(int i = 0; i < NX(); i++)
  {
    for(int j = 0; j < NY(); j++)
    {
      #pragma omp parallel for
      for(int k = 0; k < NZ(); k++)
      {
        const double dist = m_Model->minimumDistance(
          m_Grid->evalP(i,j,k));
        m_Grid->set(i,j,k,dist);
      }
    }
  }
}

void Levelset::performPass(const int pass)
{
  for(int i = 0; i < NX(pass); i++)
  {
    #pragma omp parallel for
    for(int j = 0; j < NY(pass); j++)
    {
      const Ray r = getAARay(pass, i, j);
      std::set<double> intersections;
      double lower = -DBL_MAX;
      double upper = 0.0;
     
      // Collect intersections, skip if none.
      if(!m_Model->intersects(r, intersections))
      {
        continue;
      }

      if((intersections.size() % 2) != 0)
      {
        // TODO: Alert the logger!!
        DBOUT("Hole detected!!: " << intersections.size())
      }

      std::set<double>::iterator it = intersections.begin();
      upper = *it;

      const double step = getStep(pass);
      int sign = -1;
      double rayPos = 0.0;
      for(int k = 0; k < NZ(pass); k++)
      {
        // Flip sign if needed.
        if(rayPos > upper)
        {
          sign = -sign;
          lower = *it;
          it++;

          if(it == intersections.end())
          {
            upper = DBL_MAX;
          }
          else
          {
            upper = *it;
          }
          assert(lower < upper);
        }
        
        // Don't proceed if minimum distance was run
        if(isFinal(pass, i, j, k))
        {
          rayPos += step;
          continue;
        }

        const double value = evalGrid(pass,i,j,k);
        const double currentDist = sign * std::min(
          fabs(rayPos - upper),
          fabs(rayPos - lower));
  
        assert(fabs(currentDist) < sizeZ(pass));

        if(m_Passes == 0 || fabs(currentDist) < fabs(value))
        {
          #pragma omp critical
          set(pass, i, j, k, currentDist);
        }
        
        // do triangle min-dist if less than certain amount
        if(fabs(currentDist) < m_Threshold)
        {
          const double mindist = sign * min(
              fabs(m_Model->minimumDistance(r.trace(rayPos))),
              fabs(currentDist));

          #pragma omp critical
          {
            set(pass, i, j, k, mindist);
            finalize(pass, i, j, k);
          }
        }

        rayPos += step;

      }
    }
  }

  m_Passes++;
}
