#include <cassert>

#include "Ginsu.h"
#include "Vector2D.h"
#include "macros.h"

using namespace vr;

const float Ginsu::evalPlane(const Vector2D &p) const
{
  float test_distance;
  const size_t N = m_Vectors->size();

  const Vector2D&
    p0 = (*m_Vectors)[0],
    p1 = (*m_Vectors)[1],
    pn2 = (*m_Vectors)[N-2],
    pn1 = (*m_Vectors)[N-1];

  // Compute distance from beginning and end segments. If t0 < 0 or t1 > 1,
  // this means the best answer is off of this line segment. Otherwise, it lies
  // somewhere in the middle.
  float best_distance = line_distance(p, p0, p1, 0x2);
  int best_segment = 0;

  for(size_t i = 1; i < N-1; i++)
  {
    const Vector2D&
      v = (*m_Vectors)[i],
      w = (*m_Vectors)[i+1];

    test_distance = line_distance(p, v, w, (i < N-2) ? 0x3 : 0x1);
    assert(test_distance > 0);

    if(test_distance < best_distance)
    {
      best_distance = test_distance;
      best_segment = i;
    }
  }

  const int sgn = sign(best_segment, p);

  return sgn * best_distance;
}

const int Ginsu::sign(const int i, const Vector2D &p) const
{
  const Vector2D
    &v = (*m_Vectors)[i],
    &w = (*m_Vectors)[i+1],
    dir = w - v,
    norm = Vector2D(dir.y(), -dir.x());

  const float dot = norm * (p-v);

  return 2 * (dot > 0) - 1;
}
