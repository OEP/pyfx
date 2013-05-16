#ifndef FRACTALSUMPARAMETERS_H_
#define FRACTALSUMPARAMETERS_H_

#include "Vector.h"

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
        Vector _axis = Vector::UZ,
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

}

#endif
