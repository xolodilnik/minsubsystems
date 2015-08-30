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
#ifndef MINPOINT_H_INCLUDED
#define MINPOINT_H_INCLUDED

#include <minutils/mintyp.h>
#include <minutils/crossplat.h>
#include <cmath>
#ifndef __cplusplus
/**
 * @brief   2D point <x,y> with integer/real coordinates.
 * @details @c MinPoint represents a 2D point where x and y are 32 bit integers.
 *          @c MinPoint2f represents a 2D point where x and y are 32 bit float.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  int32_t x; ///< The x-coordinate of the point
  int32_t y; ///< The y-coordinate of the point
} MinPoint;

typedef struct
{
  real32_t x; ///< The x-coordinate of the point
  real32_t y; ///< The y-coordinate of the point
} MinPoint2f;


/**
 * @brief   3D point with real coordinates (zero-based).
 * @details The structure @c MinPoint2f represents a 3D point as a triplet (x, y, z),
 *          where x, y and z are real.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  real32_t x; ///< The x-coordinate of the point.
  real32_t y; ///< The y-coordinate of the point.
  real32_t z; ///< The z-coordinate of the point.
} MinPoint3f;

#endif  // !__cplusplus

#ifdef __cplusplus
namespace se {

template < class CT > // coords type
struct pnt2
{
  CT x,y; ///< x,y - coordinates of the point
  pnt2( CT _x=0, CT _y=0 ):x(_x),y(_y){}
  template< class CT2 > 
  pnt2( const pnt2< CT2 >& p ):
    x(static_cast<CT>(p.x)),y(static_cast<CT>(p.y)){}
  CT& operator [] ( int i ) const { return (&x)[i]; }
  CT  operator [] ( int i ) { return (&x)[i]; }
  int size() { return 2; }
  template< class CT2 >  pnt2<CT>& operator *= ( const pnt2<CT2>& p ) 
  { x = static_cast<CT>(x*p.x); y=static_cast<CT>(y*p.y); return *this; }
  template< class CT2 >  pnt2<CT>& operator /= ( const pnt2<CT2>& p ) 
  { x /= p.x; y/=p.y; return *this; }
};

template < class CT > // coords type
struct pnt3
{
  CT x,y,z; ///< x,y,z - coordinates of the point
  pnt3( CT _x=0, CT _y=0, CT _z=0 ):x(_x),y(_y),z(_z){}
  template< class CT2 > 
  pnt3( const pnt3< CT2 >& p ):
    x(static_cast<CT>(p.x)),y(static_cast<CT>(p.y)),z(static_cast<CT>(p.z)){}
  template< class CT2 > 
  pnt3( const pnt2< CT2 >& p, CT2 _z ):
    x(static_cast<CT>(p.x)),y(static_cast<CT>(p.y)),z(static_cast<CT>(_z)){}
  CT& operator [] ( int i ) { return (&x)[i]; }
  CT  operator [] ( int i ) const { return (&x)[i]; }
  int size() { return 3; }
  template< class CT2 > pnt3<CT>& operator *= ( const pnt3<CT2>& p ) 
  { x *= p.x; y*=p.y; z*=p.z; return *this; }
  template< class CT2 > pnt3<CT>& operator /= ( const pnt3<CT2>& p ) 
  { x /= p.x; y/=p.y; z/=p.z; return *this; }
};

template < class CT > 
inline pnt2<CT> operator * (const pnt2<CT>& p, double alpha )
{ return pnt2<CT>(static_cast<CT>(p.x*alpha), static_cast<CT>(p.y*alpha)); }

template < class CT > 
inline pnt2<CT> operator / ( const pnt2<CT>& p, double alpha )
{ return pnt2<CT>(static_cast<CT>(p.x/alpha), static_cast<CT>(p.y/alpha)); }

template < class CT > 
inline pnt2<CT> operator * ( double alpha, const pnt2<CT>& p )
{ return pnt2<CT>(static_cast<CT>(p.x*alpha), static_cast<CT>(p.y*alpha)); }

template < class CT > 
inline pnt2<CT>& operator *= ( pnt2<CT>& p, double alpha )
{ p.x=static_cast<CT>(p.x*alpha); p.y=static_cast<CT>(p.y*alpha);  return p; }

template < class CT > 
inline pnt2<CT>& operator /= ( pnt2<CT>& p, double alpha ) 
{ p.x=static_cast<CT>(p.x/alpha); p.y=static_cast<CT>(p.y/alpha);  return p; }

template < class CT > 
inline pnt2<CT> operator - ( const pnt2<CT>& p )
{ return pnt2<CT>(-p.x, -p.y); }

template < class CT > 
inline pnt2<CT> operator + ( const pnt2<CT>& p, const pnt2<CT>& q )
{ return pnt2<CT>(p.x+q.x, p.y+q.y); }

template < class CT > 
inline pnt2<CT>& operator += ( pnt2<CT>& p, const pnt2<CT>& add_q )
{ p.x += add_q.x; p.y += add_q.y; return p;}

template < class CT > 
inline pnt2<CT> operator - ( const pnt2<CT>& p, const pnt2<CT>& q )
{ return pnt2<CT>(p.x-q.x, p.y-q.y); }

template < class CT > 
inline pnt2<CT>& operator -= ( pnt2<CT>& p, const pnt2<CT>& sub_q )
{ p.x -= sub_q.x; p.y -= sub_q.y; return p; }

template < class CT >
inline double dot( const pnt2<CT>& p, const pnt2<CT>& q )
{ double res = p.x*double(q.x)+p.y*double(q.y); return res; }

template < class CT > // z-component of cross product of 2d points
inline double cross( const pnt2<CT>& p, const pnt2<CT>& q )
{ double res = p.x*double(q.y) - p.y*double(q.x); return res; }

template < class CT > 
inline double sqlen( const pnt2<CT>& p ) // squared euclidean length
{ double res = p.x*double(p.x) + p.y*double(p.y); return res; }

template < class CT > 
inline double length( const pnt2<CT>& p ) // euclidean length
{ double res = sqrt( p.x*double(p.x) + p.y*double(p.y) ); return res; }

template < class CT > 
inline bool is_clockwise( const pnt2<CT>& a,  const pnt2<CT>& b,  const pnt2<CT>& c ) 
// returns true if triangle a-b-c is not degenerated (has non zero area)
// and points a-b-c are enumerated in clockwise order in lefthand (image) coords
{ return cross( b-a, c-a ) > 0; } // example 0,0->1,0->0,1


template < class CT > 
inline bool is_counterclockwise( const pnt2<CT>& a,  const pnt2<CT>& b,  const pnt2<CT>& c ) 
// returns true if triangle a-b-c is not degenerated (has non zero area)
// and points a-b-c are enumerated in counterclockwise order in lefthand (image) coords
{ return cross( b-a, c-a ) < 0; } // example 0,0->0,1->1,0

template < class CT >
inline double cosangle( const pnt2<CT>& a,  const pnt2<CT>& b )
// returns [-1...1] ranged cosine of angle between a and b (or between b and a, symmetric)
{
  double res = dot( a, b ) / (length(a)*length(b));
  if (res > 1.)
    res=1.;
  if (res < -1.)
    res=-1.;
  return res;
}

/////////////////////////////////////////////////////// 
// pnt3

template < class CT > 
inline pnt3<CT> operator * ( const pnt3<CT>& p, double alpha )
{ return pnt3<CT>( static_cast<CT>(p.x*alpha), static_cast<CT>(p.y*alpha), static_cast<CT>(p.z*alpha) );}

template < class CT > 
inline pnt3<CT> operator * ( double alpha, const pnt3<CT>& p )
{ return pnt3<CT>( p.x*alpha, p.y*alpha, p.z*alpha ); }

template < class CT > 
inline pnt3<CT> operator / ( const pnt3<CT>& p, double alpha )
{ return pnt3<CT>( p.x/alpha, p.y/alpha, p.z/alpha ); }

template < class CT > 
inline pnt3<CT>& operator *= ( pnt3<CT>& p, double alpha )
{ p.x *= alpha; p.y *= alpha; p.z *= alpha;  return p; }

template < class CT > 
inline pnt3<CT>& operator /= ( pnt3<CT>& p, double alpha )
{ p.x /= alpha; p.y /= alpha; p.z /= alpha;  return p; }

template < class CT > 
inline pnt3<CT> operator - ( const pnt3<CT>& p )
{ return pnt3<CT>( -p.x, -p.y, -p.z ); }

template < class CT > 
inline pnt3<CT> operator + ( const pnt3<CT>& p, const pnt3<CT>& q )
{ return pnt3<CT>( p.x+q.x, p.y+q.y, p.z+q.z ); }

template < class CT > 
inline pnt3<CT>& operator += ( pnt3<CT>& p, const pnt3<CT>& add_q )
{ p.x += add_q.x; p.y += add_q.y; p.z += add_q.z;  return p;}

template < class CT > 
inline pnt3<CT> operator - ( const pnt3<CT>& p, const pnt3<CT>& q )
{ return pnt3<CT>(p.x-q.x, p.y-q.y, p.z-q.z); }

template < class CT > 
inline pnt3<CT>& operator -= ( pnt3<CT>& p, const pnt3<CT>& sub_q )
{ p.x -= sub_q.x; p.y -= sub_q.y; p.z -= sub_q.z;  return p; }

template < class CT > 
inline double dot( const pnt3<CT>& p, const pnt3<CT>& q )
{ double res = p.x*double(q.x)+p.y*double(q.y)+p.z*double(q.z); return res; }

template < class CT > 
inline pnt3<CT> cross( const pnt3<CT>& p, const pnt3<CT>& q )
{ 
  pnt3<CT> res( 
    p.y*q.z - p.z*q.y, 
    p.z*q.x - p.x*q.z, 
    p.x*q.y - p.y*q.x 
  );
  return res; 
}

template < class CT > 
inline double sqlen( const pnt3<CT>& p ) // squared euclidean length
{ double res = p.x*double(p.x) + p.y*double(p.y) + p.z*double(p.z); return res; }

template < class CT > 
inline double length( const pnt3<CT>& p ) // euclidean length
{ double res = sqrt( p.x*double(p.x) + p.y*double(p.y) + p.z*double(p.z) ); return res; }

template < class CT >
inline double cosangle( const pnt3<CT>& a,  const pnt3<CT>& b )
// returns [-1...1] ranged cosine of angle between a and b (or between b and a, symmetric)
{
  double res = dot( a, b ) / (length(a)*length(b));
  if (res > 1.)
    res=1.;
  else if (res < -1.)
    res=-1.;
  return res;
}

//inline pnt3<CT> xyz( const pnt2<CT>& xy, float z ) { pnt3<CT> res={ xy.x, xy.y, z }; return res; }
//inline pnt3<CT> xyz( const MinPoint& xy, float z ) { pnt3<CT> res={ float(xy.x), float(xy.y), z }; return res; }
//inline pnt2<CT> xy( const pnt3<CT>& xyz ) {  pnt2<CT> res={ xyz.x, xyz.y }; return res; }
//inline pnt2<CT> xy( const MinPoint& xy ) {  pnt2<CT> res={ float(xy.x), float(xy.y) }; return res; }
// use round()? inline MinPoint minPoint( const pnt3<CT>& xyz ) {  MinPoint res={ xyz.x, xyz.y }; return res; }
// use round()? inline MinPoint minPoint( const pnt2<CT>& xy ) {  pnt2<CT> res={ xy.x, xy.y }; return res; }

}; // namespace se 

typedef se::pnt2< int32_t > MinPoint2i;
typedef se::pnt3< int32_t > MinPoint3i;
typedef se::pnt2< int16_t > MinPoint2s;
typedef se::pnt3< int16_t > MinPoint3s;
typedef se::pnt2< real32_t > MinPoint2f;
typedef se::pnt3< real32_t > MinPoint3f;
typedef se::pnt2< real64_t > MinPoint2d;
typedef se::pnt3< real64_t > MinPoint3d;

typedef MinPoint2i MinPoint;

#endif // __cplusplus

/**
 * @brief   Inline constructors for @c MinPoint data type.
 * @param   x The x-coordinate of the point.
 * @param   y The y-coordinate of the point.
 * @returns A new @c MinPoint object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinPoint object using x-coordinate and
 * y-coordinate values.
 */
MUSTINLINE MinPoint minPoint(int32_t x, int32_t y)
{
#ifndef __cplusplus
  MinPoint point = {x, y};  return point;
#else
  return MinPoint(x,y);
#endif
}

/**
 * @brief   Inline constructors for @c MinPoint2f data type.
 * @param   x The x-coordinate of the point.
 * @param   y The y-coordinate of the point.
 * @returns A new @c MinPoint2f object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinPoint2f object using x-coordinate and
 * y-coordinate values.
 */
MUSTINLINE MinPoint2f minPoint2f(real32_t x, real32_t y)
{
#ifndef __cplusplus
  MinPoint2f point = {x, y};  return popathint;
#else
  return MinPoint2f(x,y);
#endif
}

/**
 * @brief   Inline constructors for @c MinPoint3f data type.
 * @param   x The x-coordinate of the point.
 * @param   y The y-coordinate of the point.
 * @param   z The y-coordinate of the point.
 * @returns A new @c MinPoint3f object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinPoint3f object using x-coordinate and
 * y-coordinate values.
 */
MUSTINLINE MinPoint3f minPoint3f(real32_t x, real32_t y, real32_t z)
{
#ifndef __cplusplus
  MinPoint3f point = {x, y, z};  return point;
#else
  return MinPoint3f(x, y, z);
#endif
}

#endif // MINPOINT_H_INCLUDED
