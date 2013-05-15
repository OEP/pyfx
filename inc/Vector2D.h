#ifndef VECTOR2D_H_
#define VECTOR2D_H_

#include <ostream>

using namespace std;

namespace vr
{
  class Vector2D
  {
    private:
      double m_XY[3];

    protected:
    public:
      static const int
        COMPONENT_X, COMPONENT_Y;

      static const Vector2D
        MAX, MIN,
        ZEROS, ONES,
        AXIS_X, AXIS_Y;

      Vector2D();
      Vector2D(const Vector2D&);
      Vector2D(const double x, const double y);

      const double length() const;
      const Vector2D unit() const;
      void normalize();

      const Vector2D componentMin(const Vector2D&) const;
      const Vector2D componentMax(const Vector2D&) const;
      const Vector2D componentProduct(const Vector2D&) const;
      const Vector2D replace(const int i, const double x) const;

      const double x() const;
      const double y() const;

      const bool equals(const Vector2D&) const;

      const Vector2D operator- ()                const;
      const Vector2D operator- (const Vector2D& v) const;
      const Vector2D operator/ (const double)    const;
      const Vector2D operator/ (const Vector2D& v) const;
      const Vector2D operator+ (const Vector2D& v) const;
      const double operator* (const Vector2D& v) const;
      const Vector2D operator* (const double)    const;

      const double operator[](const int) const;

      const bool operator== (const Vector2D& v) const;
      const bool operator!= (const Vector2D& v) const;
      const bool operator>= (const Vector2D& v) const;
      const bool operator<= (const Vector2D& v) const;
      const bool operator>  (const Vector2D& v) const;
      const bool operator<  (const Vector2D& v) const;

      const std::string toString() const;
      const std::string __str__() const;
      const std::string __repr__() const;

      friend ostream& operator<< (ostream& output, const Vector2D& v);
      friend const Vector2D operator* (const double, const Vector2D& v);
  };
  
  ostream& operator<< (ostream& output, const Vector2D& v);
  const Vector2D operator*(const double, const Vector2D& v);

  const float line_distance(
    const Vector2D&,
    const Vector2D&,
    const Vector2D&,
    const int unbounded_flags = 0x0
  );
}


#endif
