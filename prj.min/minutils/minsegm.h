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
#ifndef MINSEGM_H_INCLUDED
#define MINSEGM_H_INCLUDED

#include <minutils/mintyp.h>
#include <minutils/crossplat.h>

#include <minutils/minpoint.h>
/**
 * @brief   Line segment represented as two ends points.
 * @details The struct @c MinLineSegment represents a line segment as a tuple of
 * two MinPoint's.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  MinPoint u;     ///< First point.
  MinPoint v;     ///< Second point.
} MinLineSegment;

/**
 * @brief   Inline constructor for @c MinLineSegment data type.
 * @param   u      First point.
 * @param   v      Second point.
 * @returns A new @c MinLineSegment object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinLineSegment object using two point.
 */
MUSTINLINE MinLineSegment minLineSegment(MinPoint u, MinPoint v)
{
  MinLineSegment segment = {u, v};
  return segment;
}

//---------------------------------------------------------- new 23 feb 2012
#include <minutils/minpoint.h>

#ifndef __cplusplus
/**
 * @brief   2D segment <a,b> of <x,y> points with integer/real coordinates.
 * @details @c MinSegm2[s|i|f|d] represents a 2D segment of line where a and b are 2d endpoints.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  MinPoint a; ///< staring 2d point (integer coords)
  MinPoint b; ///< ending 2d point (integer coords)
} MinSegm;

typedef struct
{
  MinPoint2f a; ///< staring 2d point (real coords, single precision)
  MinPoint2f b; ///< ending 2d point (real coords, single precision)
} MinSegm2f;


#endif  // !__cplusplus

#ifdef __cplusplus
namespace se {

template < class CT > // coords type
struct segm2
{
  pnt2< CT > a,b; ///< a,b - end points of 2d line segment
  segm2( CT _x1=0, CT _y1=0,  CT _x2=0, CT _y2=0 ):a(_x1, _y1),b(_x2, _y2){}

  template< class CT2 > 
  segm2( const segm2< CT2 >& s ):
    a(s.a), b(s.b){}  ///a(static_cast<CT>(s.a)),b(static_cast<CT>(s.b)){}

  template< class CT2 > 
  segm2( const pnt2< CT2 >& _a,  const pnt2< CT2 >& _b ):
    a(_a),b(_b){}

  pnt2< CT >& operator [] ( int i ) { return (&a)[i]; }
  int size() { return 2; }
  template< class CT2 >  segm2<CT>& operator *= ( const pnt2<CT2>& p ) 
  { a *= p; b*=p; return *this; }
  template< class CT2 >  segm2<CT>& operator /= ( const pnt2<CT2>& p ) 
  { a /= p; b/=p; return *this; }
};

template < class CT > // coords type
struct segm3
{
  pnt3< CT > a,b;  ///< a,b - end points of 3d line segment
  segm3( 
    CT _x1=0, CT _y1=0, CT _z1=0,  
    CT _x2=0, CT _y2=0, CT _z2=0  ):
    a(_x1, _y1, _z1), 
    b(_x2, _y2, _z2)
    {}

  template< class CT2 > 
  segm3( const segm3< CT2 >& s ):
    a(s.a), b(s.b){}  ///a(static_cast<CT>(s.a)),b(static_cast<CT>(s.b)){}

  template< class CT2 > 
  segm3( const pnt3< CT2 >& _a,  const pnt3< CT2 >& _b ):
    a(_a),b(_b){}

  pnt3< CT >& operator [] ( int i ) { return (&a)[i]; }
  int size() { return 2; }
  template< class CT2 >  segm3<CT>& operator *= ( const pnt3<CT2>& p ) 
  { a *= p; b*=p; return *this; }
  template< class CT2 >  segm3<CT>& operator /= ( const pnt3<CT2>& p ) 
  { a /= p; b/=p; return *this; }
};

template < class CT > 
inline segm2<CT>& operator *= ( segm2<CT>& p, double alpha )
{  p.a *= alpha; p.b *= alpha; return p; }

template < class CT > 
inline segm3<CT>& operator *= ( segm3<CT>& p, double alpha )
{  p.a *= alpha; p.b *= alpha; return p; }

template < class CT > 
inline segm2<CT>& operator /= ( segm2<CT>& p, double alpha )
{ p.a /= alpha; p.b /= alpha; return p; }

template < class CT > 
inline segm3<CT>& operator /= ( segm3<CT>& p, double alpha )
{ p.a /= alpha; p.b /= alpha; return p; }

template < class CT > 
inline double length( const segm2<CT>& s ) // euclidean length
{ double res = length( s.b-s.a ); return res; }

template < class CT > 
inline double length( const segm3<CT>& s ) // euclidean length
{ double res = length( s.b-s.a ); return res; }

template < class CT > 
inline double sqlen( const segm2<CT>& s ) // squared euclidean length
{ double res = sqlen( s.b-s.a );  return res; }

template < class CT > 
inline double sqlen( const segm3<CT>& s ) // squared euclidean length
{ double res = sqlen( s.b-s.a );  return res; }

}; // namespace se::

typedef se::segm2< int16_t > MinSegm2s;
typedef se::segm2< int32_t > MinSegm2i;
typedef se::segm2< real32_t > MinSegm2f;
typedef se::segm2< real64_t > MinSegm2d;

typedef MinSegm2i MinSegm;

typedef se::segm3< int16_t > MinSegm3s;
typedef se::segm3< int32_t > MinSegm3i;
typedef se::segm3< real32_t > MinSegm3f;
typedef se::segm3< real64_t > MinSegm3d;

#endif // __cplusplus
//---------------------------------------------------------- end of new  23 feb 2012

#endif // MINSEGM_H_INCLUDED
