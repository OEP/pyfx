#ifndef MACROS_H_
#define MACROS_H_

#include <iostream>
#include <cmath>

#ifndef M_PI
  #define M_PI 3.14159265359
#endif

#ifdef RELEASE
  #define DBOUT( x )
#else
  #define DBOUT( x ) std::cout << x << std::endl;
#endif

#ifndef DEG2RAD
  #define DEG2RAD( x ) x * M_PI / 180.0
#endif

#ifndef RAD2DEG
  #define RAD2DEG( x ) x * 180.0 / M_PI
#endif

#ifndef SQUARE
  #define SQUARE(x) x * x
#endif

#ifndef HYPOT
  #define HYPOT(x,y) sqrt( x * x + y * y )
#endif

#ifndef LERP
  #define LERP(x,y,q) (x * (1-q) + y * q)
#endif

#ifndef SAFE_NORMALIZE
  #define SAFE_NORMALIZE(p,c) (c > 0) ? p / c : p
#endif

#endif
