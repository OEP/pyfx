#include "Triangle.h"
#include "macros.h"

using namespace vr;

Triangle::Triangle(const Vector &a, const Vector &b, const Vector &c)
{
  m_Points[0] = a;
  m_Points[1] = b;
  m_Points[2] = c;
  update();
}

Triangle::Triangle(const Triangle &t)
{
  m_Points[0] = t.m_Points[0];
  m_Points[1] = t.m_Points[1];
  m_Points[2] = t.m_Points[2];
  update();
}

void Triangle::update()
{
  m_Normal = ((m_Points[1] - m_Points[0]) ^ (m_Points[2] - m_Points[0])).unit();
  m_D = m_Normal * m_Points[0];
  m_Edge[0] = m_Points[2] - m_Points[0];
  m_Edge[1] = m_Points[1] - m_Points[0];

  m_Dot00 = m_Edge[0] * m_Edge[0];
  m_Dot01 = m_Edge[0] * m_Edge[1];
  m_Dot11 = m_Edge[1] * m_Edge[1];
  m_InvDenom = 1 / (m_Dot00 * m_Dot11 - m_Dot01 * m_Dot01);
}

const Vector& Triangle::normal() const
{
  return m_Normal;
}

const double Triangle::intersects(const Ray &r) const
{
  double dump;
  if(!intersects(r,dump))
  {
    return -1.0;
  }
  return dump;
}

const double Triangle::minimumVertexDistance(const Vector &p) const
{
  return
    std::min(  (p - m_Points[0]).magnitude(),
      std::min((p - m_Points[1]).magnitude(),
               (p - m_Points[2]).magnitude()));
}

const double Triangle::minimumDistance(const Vector &p) const
{
  return minimumVertexDistance(p);
}

/*
const double Triangle::minimumDistance(const Vector &p) const
{
  // From:
  // http://www.geometrictools.com/Documentation/DistancePoint3Triangle3.pdf
  //
  // This code based partially on the given paper and the matlab
  // implementation by Gwendolyn Fischer available at:
  // http://www.mathworks.com/matlabcentral/fileexchange/22857-distance-between-a-point-and-a-triangle-in-3d
  // under the BSD license.
  const Vector
    D = m_Points[0] - p;
  const double
    a = m_Dot00,
    b = m_Dot01,
    c = m_Dot11,
    d = m_Edge[0] * D,
    e = m_Edge[1] * D,
    f = D * D,
    det = a*c - b*b;

  // s and t are the barycentric coordinates of nearest point
  double
    s = b*e - c*d,
    t = b*d - a*e,
    sqrDistance = 0.0,
    numer, denom, tmp0, tmp1;

  // Performance-tanking conditional tree, ho!
  if((s+t) <= det)
  {
    if(s < 0)
    {
      if(t < 0)
      {
        // region4
        if(d < 0)
        {
          t = 0;
          if(-d >= a)
          {
            s = 1;
            sqrDistance = a + 2*d + f;
          }
          else
          {
            s = -d/a;
            sqrDistance = d*s + f;
          }
        }
        else
        {
          s = 0;
          if (e >= 0)
          {
            t = 0;
            sqrDistance = f;
          }
          else
          {
            if (-e >= c)
            {
              t = 1;
              sqrDistance = c + 2*e + f;
            }
            else
            {
              t = -e/c;
              sqrDistance = e*t + f;
            }
          }
        } // end of region 4
      }
      else
      {
        // region 3
        s = 0;
        if(e >= 0)
        {
          t = 0;
          sqrDistance = f;
        }
        else
        {
          if(-e >= c)
          {
            t = 1;
            sqrDistance = c + 2*e +f;
          }
          else
          {
            t = -e/c;
            sqrDistance = e*t + f;
          }
        }
      } // end of region 3
    }
    else
    {
      if(t < 0)
      {
        // region 5
        t = 0;
        if(d >= 0)
        {
          s = 0;
          sqrDistance = f;
        }
        else
        {
          if(-d >= a)
          {
            s = 1;
            sqrDistance = a + 2*d + f;
          }
          else
          {
            s = -d/a;
            sqrDistance = d*s + f;
          }
        }
      } // end region 5
      else
      {
        s = s*m_InvDenom;
        t = t*m_InvDenom;
        sqrDistance = s*(a*s + b*t + 2*d)
                    + t*(b*s + c*t + 2*e) + f;
      }
    }
  }
  else
  {
    if(s < 0)
    {
      // region 2
      tmp0 = b + d;
      tmp1 = c + e;
      if(tmp1 > tmp0) // minimum on edge s+t=1
      {
        numer = tmp1 - tmp0;
        denom = a - 2*b + c;
        if(numer >= denom)
        {
          s = 1;
          t = 0;
          sqrDistance = a + 2*d + f;
        }
        else
        {
          s = numer/denom;
          t = 1-s;
          sqrDistance = s*(a*s + b*t + 2*d)
                      + t*(b*s + c*t + 2*e) + f;
        }
      }
      else
      {
        s = 0;
        if(tmp1 <= 0)
        {
          t = 1;
          sqrDistance = c + 2*e + f;
        }
        else
        {
          if(e >= 0)
          {
            t = 0;
            sqrDistance = f;
          }
          else
          {
            t = -e/c;
            sqrDistance = e*t + f;
          }
        }
      } // end region 2
    }
    else
    {
      if(t < 0)
      {
        // region 6
        tmp0 = b + e;
        tmp1 = a + d;
        if (tmp1 > tmp0)
        {
          numer = tmp1 - tmp0;
          denom = a-2*b+c;
          if (numer >= denom)
          {
            t = 1;
            s = 0;
            sqrDistance = c + 2*e + f;
          }
          else
          {
            t = numer/denom;
            s = 1 - t;
            sqrDistance = s*(a*s + b*t + 2*d)
                        + t*(b*s + c*t + 2*e) + f;
          }
        }
        else  
        {
          t = 0;
          if (tmp1 <= 0)
          {
            s = 1;
            sqrDistance = a + 2*d + f;
          }
          else
          {
            if (d >= 0)
            {
              s = 0;
              sqrDistance = f;
            }
            else
            {
              s = -d/a;
              sqrDistance = d*s + f;
            }
          }
        } // end region 6
      }
      else
      {
        // region 1
        numer = c + e - b - d;
        if(numer <= 0)
        {
          s = 0;
          t = 1;
          sqrDistance = c + 2*e + f;
        }
        else
        {
          denom = a - 2*b + c;
          if(numer >= denom)
          {
            s = 1;
            t = 0;
            sqrDistance = a + 2*d + f;
          }
          else
          {
            s = numer/denom;
            t = 1-s;
            sqrDistance = s*(a*s + b*t + 2*d)
                        + t*(b*s + c*t + 2*e) + f;
          }
        }
      }
    }
  }

  return sqrt(sqrDistance);
}
*/

const bool Triangle::intersects(const Ray &r, double &t) const
{
  if(normal() * r.direction() == 0.0)
  {
    return false;
  }
  t = (m_D - normal() * r.origin()) / (normal() * r.direction());

  if(t < 0)
  {
    return false;
  }

  const Vector Q = r.trace(t);
  return contains(Q);
}
      
const Vector& Triangle::point(const int i) const
{
  return m_Points[i];
}

const bool Triangle::contains(const Vector &p) const
{
  const Vector e2 = p - m_Points[0];
  const double
    dot02 = m_Edge[0] * e2,
    dot12 = m_Edge[1] * e2,
    u = (m_Dot11 * dot02 - m_Dot01 * dot12) * m_InvDenom,
    v = (m_Dot00 * dot12 - m_Dot01 * dot02) * m_InvDenom;

  return (u >= 0) && (v >= 0) && (u + v < 1);
}
