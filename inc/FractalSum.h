#ifndef FRACTALSUM_H_
#define FRACTALSUM_H_

#include "Vector.h"
#include "Noise.h"

namespace vr
{
  class FractalSumParameters
  {
    public:
      FractalSumParameters(
        double _octaves = 3.0,
        double _fjump = 2.0,
        double _roughness = 0.5,
        double _frequency = 0.6666,
        Vector _translate = Vector(0),
        double _offset = 0.0,
        Vector _axis = vector::UZ,
        double _angle = 0.0
      ):
        octaves(_octaves),
        fjump(_fjump),
        roughness(_roughness),
        frequency(_frequency),
        translate(_translate),
        offset(_offset),
        axis(_axis),
        angle(_angle)
      {}
        
      double octaves;
      double fjump;
      double roughness;
      double frequency;
      Vector translate;
      double offset;
      Vector axis;
      double angle;

      const FractalSumParameters interpolate(const FractalSumParameters &other, float q) const;
  };


  class FractalSum : public Noise
  {
    private:
    protected:
    public:
      FractalSum(const Noise *n = NULL,
        const FractalSumParameters params = FractalSumParameters());

      void setParameters(const FractalSumParameters &p);
      const double calcMax(const double) const;
  };
}

#endif
