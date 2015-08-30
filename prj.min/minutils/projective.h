/*

Copyright (c) 2011, Smart Engines Limited. All rights reserved. 

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of copyright holders.

*/

#pragma once
#ifndef PROJECTIVE_H_INCLUDED
#define PROJECTIVE_H_INCLUDED

#include <minutils/minpoint.h>
#include <minutils/minquad.h>
//?#include <algorithm>

class HCoords // image to homogenious coords translation
{
private:
	double xwi; ///< image width
	double yhe; ///< image height
	double zde; ///< z-depth // 0 -> auto -> sqrt( wi*wi+he*he )  // depth of view point; positive
public:
  double x() const { return xwi; }
  double y() const { return yhe; }
  double z() const { return zde; }

  double image_width() const { return xwi; }
  MinPoint2d image_size() const { return MinPoint2d(xwi, yhe); }
  double image_height() const { return yhe; }
  double focal_length() const { return zde; }

  void setup( double _xwi, double _yhe )
  {
    xwi = _xwi;
    yhe = _yhe;
//#define LIKE_IPHONE_CAMERA
#ifdef LIKE_IPHONE_CAMERA
    zde = (1.732050) * std::max( xwi, yhe );
#else  // old version
    zde = 0.5*sqrt( xwi*xwi+yhe*yhe );
#endif
  }

  HCoords(){ setup(1,1); }
  HCoords( double xwi, double yhe ) { setup( xwi, yhe ); }

  MinPoint2d to2d( const MinPoint3d& a ) { return MinPoint2d( a.x + xwi/2.,  a.y + yhe/2. ); }
  MinPoint3d to3d( const MinPoint2f& a ) { return MinPoint3d( a.x - xwi/2.,  a.y - yhe/2., zde ); }

};


namespace se {

// different focal lenths
// use: image width * value
#define FOCAL_LENGTH_DEFAULT 0.625
#define FOCAL_LENGTH_IPH4 0.838
#define FOCAL_LENGTH_IPH4S 0.932


template< class CT >
inline bool restore_parallelogram_by_projection( 
  const quad2< CT >& prj, // projection
  quad3< CT >& res, // result parallelogram 
  const CT& flen, // known focal length
  double& angle_to_normal // angle to quadrangle plane in radians
  )
{
  pnt3<CT> p( prj.a.x-prj.b.x,  prj.c.x-prj.b.x, prj.b.x-prj.d.x );
  pnt3<CT> q( prj.a.y-prj.b.y,  prj.c.y-prj.b.y, prj.b.y-prj.d.y );
  pnt3<CT> m = cross( p, q );
  if (std::abs(m.z) < 0.000000001) // todo use EPS...
    return false;
  m /= m.z;
  double mx = static_cast<double>(m.x) / m.z;
  double my = static_cast<double>(m.y) / m.z;
  double lambda1 = mx;
  double lambda3 = my;
  double lambda2 = lambda1 + lambda3 - 1.0;
  double lambda4 = 1.0;

  if (lambda1 <= 0.0 || lambda2 <= 0.0 || lambda3 <= 0.0) // assuming that lambda4 > 0 ( == 1)
    return false;
  
  res.a = pnt3<CT>(prj.a, flen) * lambda1; 
  res.b = pnt3<CT>(prj.b, flen) * lambda2; 
  res.c = pnt3<CT>(prj.c, flen) * lambda3; 
  res.d = pnt3<CT>(prj.d, flen) * lambda4; 

  // new:
  pnt3<CT> u = res.b - res.a;
  pnt3<CT> v = res.c - res.a;
  pnt3<CT> n = cross( u, v );
  pnt3<CT> e( 0, 0, 1);
  double dd = dot( n, e );
  double nl = length( n );
  dd /= nl;
  angle_to_normal = acos( dd );
  //std::cout << angle_to_normal * 180/3.1415 << std::endl;





  return true;
}


template< class CT >
inline bool restore_focal_length_by_projection_of_rectangle( 
  const quad2< CT >& prj, // projection of rectangle
  CT& flen // focal length to compute
  )
{
  pnt3< CT > p( prj.a.x-prj.b.x,  prj.c.x-prj.b.x, prj.b.x-prj.d.x );
  pnt3< CT > q( prj.a.y-prj.b.y,  prj.c.y-prj.b.y, prj.b.y-prj.d.y );
  pnt3< CT > m = cross( p, q );
  if (std::abs(m.z) < 0.000000001) // todo use EPS...
    return false;
  m /= m.z;
  CT lambda1 = m.x;
  CT lambda3 = m.y;
  CT lambda2 = lambda1 + lambda3 - 1;
  CT lambda4 = 1;

  if (lambda1 <= 0 || lambda2 <=0 || lambda3 <=0 ) // assuming that lambda4 > 0 ( == 1)
    return false;

  quad3< CT > res;
  res.a = pnt3< CT >( prj.a, flen ) * lambda1; 
  res.b = pnt3< CT >( prj.b, flen ) * lambda2; 
  res.c = pnt3< CT >( prj.c, flen ) * lambda3; 
  res.d = pnt3< CT >( prj.d, flen ) * lambda4; 
  
  double f2 = - ((res.b.x - res.a.x) * (res.d.x - res.a.x) + (res.b.y - res.a.y) * (res.d.y - res.a.y))
    / ((lambda2 - lambda1) * (lambda4 - lambda1));

  if (f2 < 0)
    f2 = -f2;//return false;

  flen = sqrt( f2 );

  return true;
}


template< class CT >
inline bool restore_rectangle_by_projection( 
  const quad2< CT >& prj, // projection of rectangle
  quad3< CT >& res, // restored rectangle
  CT& flen // focal length to compute
  )
{
  pnt3< CT > p( prj.a.x-prj.b.x,  prj.c.x-prj.b.x, prj.b.x-prj.d.x );
  pnt3< CT > q( prj.a.y-prj.b.y,  prj.c.y-prj.b.y, prj.b.y-prj.d.y );
  pnt3< CT > m = cross( p, q );
  if ( std::abs(m.z) < 0.000000001 ) // todo use EPS...
    return false;
  m /= m.z;
  CT lambda1 = m.x;
  CT lambda3 = m.y;
  CT lambda2 = lambda1+lambda3-1;
  CT lambda4 = 1;

  if (lambda1 <= 0 || lambda2 <=0 || lambda3 <=0 ) // assuming that lambda4 > 0 ( == 1)
    return false;

  res.a = pnt3< CT >( prj.a, flen ) * lambda1; 
  res.b = pnt3< CT >( prj.b, flen ) * lambda2; 
  res.c = pnt3< CT >( prj.c, flen ) * lambda3; 
  res.d = pnt3< CT >( prj.d, flen ) * lambda4; 
  
  double f2 = - ((res.b.x - res.a.x) * (res.d.x - res.a.x) + (res.b.y - res.a.y) * (res.d.y - res.a.y))
    / ((lambda2 - lambda1) * (lambda4 - lambda1));
  //if (f2 < 0)
  //  return false;

  flen = sqrt( std::abs(f2) );

  res.a.z = flen*lambda1;
  res.b.z = flen*lambda2;
  res.c.z = flen*lambda3;
  res.d.z = flen*lambda4;

  return true;
}

template< class CT >
inline bool restore_4th_side_by_3_and_r( 
  const pnt2< CT >& a, // point a
  const pnt2< CT >& b, // point b
  const pnt3< CT >& p, // vector to the intersection point
  const double& r, // sides ratio
  CT& flen, // focal length 
  quad2< CT >& res // result
  )
{
  pnt3< CT > pp = p / length( p );
  double t4;
  pnt3< CT > c, d, qq, tmp1, cc, dd;
  pnt3< CT > aa(a, flen);
  pnt3< CT > bb(b, flen);
  res.a = a;
  res.b = b;

  if (fabs(dot( pp, (bb - aa) ) ) <  1e-8)
    qq = (bb - aa) / length (bb - aa);
  else
    qq = aa - dot( pp, aa ) / dot( pp, (bb - aa) ) * (bb - aa);  tmp1 = cross( pp, qq );
 
  bb *= dot( tmp1, aa ) / dot( tmp1, bb );

  t4 = r * length( aa - bb );
  cc = bb + pp * t4;
  dd = aa + pp * t4;

  cc *= flen / cc.z;
  dd *= flen / dd.z;

  res.c.x = cc.x; res.c.y = cc.y;
  res.d.x = dd.x; res.d.y = dd.y;
  return true;
}



}; // namespace se 

#endif // PROJECTIVE_H_INCLUDED


