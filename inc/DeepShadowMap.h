#ifndef DEEPSHADOWMAP_H_
#define DEEPSHADOWMAP_H_

#include <vector>

#include "Griddable.h"
#include "DenseGrid.h"
#include "UniformPRN.h"

namespace vr
{
  class DeepShadowMap
  {
    public:
      static const float DEFAULT_VALUE;
    private:

      int m_Resolution;
      int m_Samples;
      double m_Jitter;

      std::vector<Camera*> m_Garbage;
      std::vector<ScalarDenseGrid*> m_Grids;
      std::vector<Camera*> m_Griddables;

      UniformPRN m_PRN;

    protected:
      const Vector pickRandom(UniformPRN &prn, const int i, const int j, const int k) const;
      void computeSamples(const int index,
        const Vector &pl,
        const int grid,
        const double ds,
        const ScalarField *dns);
      const float computeRay(
        const int i,
        const int j,
        const int k,
        const int sample,
        const Vector &pl,
        const int grid,
        const double ds,
        const ScalarField *dns);

      const bool canSkip(int i, int j, int k, int grid, const ScalarField *dns) const;
      const bool hasDensity(int i, int j, int k, int grid, const ScalarField *dns) const;
    public:
      DeepShadowMap(const int resolution)
        : m_Resolution(resolution), m_Samples(10), m_Jitter(1.0/resolution)
      {
        m_PRN.setSeed(0xD5);
      }
      ~DeepShadowMap();


      const int add(const Vector &pl, const double ds, const ScalarField *dns, Camera *c = NULL);
      const int size() const;
      const float eval(const int i, const Vector &x) const;
      ScalarDenseGrid* getMap(const int i) const;

      void setSamples(const int);
      void setSeed(const unsigned long);
  };
}

#endif
