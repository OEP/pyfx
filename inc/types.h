#ifndef TYPES_H_
#define TYPES_H_
#include <map>
#include <vector>

#include "Volume.h"
#include "Vector2D.h"

namespace vr
{
  typedef std::map<std::string, std::string> Properties;

  typedef std::vector<ScalarField*> ScalarVolumeSet;

  typedef std::vector<Vector> VectorSequence;
  typedef std::vector<Vector2D> Vector2DSequence;
}

#endif
