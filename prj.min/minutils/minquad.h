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
#ifndef MINQUAD_H_INCLUDED
#define MINQUAD_H_INCLUDED

#include <minutils/mintyp.h>
#include <minutils/minpoint.h>
#include <algorithm>

/**
 * @brief   2D plain quadrangle with real (..2f, ..2d suffixes) or int (..2s, ..2i) coordinates.
 * @details The structure @c Minquad2f represents a 2D quadrangle as a quartet <a,b,c,d> 
 *          where a, b, c, d are 2D real points MinPoint2x (x = f,d,s,i).
 * @ingroup MinUtils_Geometry
 */

#ifndef __cplusplus

typedef struct { MinPoint2f a,b,c,d; } Minquad2f; // float32
typedef struct { MinPoint2i a,b,c,d; } Minquad2i; // int32
typedef struct { MinPoint2s a,b,c,d; } Minquad2s; // int16
typedef struct { MinPoint2d a,b,c,d; } Minquad2d; // float64

//inline MUSTINLINE Minquad2f minquad2f(MinPoint2f a, MinPoint2f b, MinPoint2f c, MinPoint2f d)
//{
//  Minquad2f q = {a,b,c,d};
//  return q;
//}

#endif  // !__cplusplus

#ifdef __cplusplus
namespace se {

#ifndef MAXMIN4_DEFINED
  using std::max;
  using std::min;
  template <class CT>
  CT max4( const CT& x1, const CT& x2, const CT& x3, const CT& x4 ) // CT& -- ?
  {
    return std::max( std::max( x1, x2 ), std::max( x3, x4 ) );
  }
  template <class CT>
  CT min4( const CT& x1, const CT& x2, const CT& x3, const CT& x4 ) // CT& -- ?
  {
    return std::min( std::min( x1, x2 ), std::min( x3, x4 ) );
  }
#define MAXMIN4_DEFINED
#endif

template <class CT>
class quad2
{
public:
  pnt2<CT> a,b,c,d; ///< 2d nodes of quadrangle <a,b,c,d>;
  quad2<CT>(){}
  template <class CT2>
  quad2<CT>( const pnt2<CT2>& a, const pnt2<CT2>& b, const pnt2<CT2>& c, const pnt2<CT2>& d ):
    a(a),b(b),c(c),d(d){}
  template <class CT2>
  quad2<CT>( const quad2<CT2>& q ) : a(q.a), b(q.b), c(q.c), d(q.d) {}
  pnt2<CT>& operator [] ( int i ) { return (&a)[i]; } ///< access to <a,b,c,d> as array[0..3];
  const pnt2<CT>& operator [] ( int i ) const { return (&a)[i]; } ///< const access to <a,b,c,d> as array[0..3];
  int size() const { return 4; }
};

template <class CT>
class quad3
{
public:
  pnt3<CT> a,b,c,d; ///< 3d nodes of quadrangle <a,b,c,d>;
  quad3<CT>(){}
  template <class CT2>
  quad3<CT>( const pnt3<CT2>& a, const pnt3<CT2>& b, const pnt3<CT2>& c, const pnt3<CT2>& d ):
  a(a),b(b),c(c),d(d){}
  pnt3<CT>& operator [] ( int i ) { return (&a)[i]; } ///< access to <a,b,c,d> as array[0..3];
  const pnt3<CT>& operator [] ( int i ) const { return (&a)[i]; } ///< access to <a,b,c,d> as array[0..3];
  int size() const { return 4; }
};
//======= mul scalar
template < class CT > 
inline quad2<CT> operator * ( const quad2<CT>& p, double alpha ) 
{ return quad2<CT>(p.a*alpha, p.b*alpha, p.c*alpha, p.d*alpha); }

template < class CT > 
inline quad2<CT> operator * ( double alpha, const quad2<CT>& p ) 
{ return quad2<CT>(p.a*alpha, p.b*alpha, p.c*alpha, p.d*alpha); }

template < class CT > 
inline quad3<CT> operator * ( const quad3<CT>& p, double alpha ) 
{ return quad3<CT>(p.a*alpha, p.b*alpha, p.c*alpha, p.d*alpha); }

template < class CT > 
inline quad3<CT> operator * ( double alpha, const quad2<CT>& p ) 
{ return quad3<CT>(p.a*alpha, p.b*alpha, p.c*alpha, p.d*alpha); }

template < class CT > 
inline quad2<CT>& operator *= ( quad2<CT>& p, double alpha ) 
{ p.a *= alpha; p.b *= alpha;  p.c *= alpha; p.d *= alpha;  return p; }

template < class CT > 
inline quad3<CT>& operator *= ( quad3<CT>& p, double alpha ) 
{ p.a *= alpha; p.b *= alpha;  p.c *= alpha; p.d *= alpha;  return p; }

//======== div scalar
template < class CT > 
inline quad2<CT> operator / ( const quad2<CT>& p, double alpha )
{ return quad2<CT>(p.a/alpha, p.b/alpha, p.c/alpha, p.d/alpha); }

template < class CT > 
inline quad2<CT>& operator /= ( quad2<CT>& p, double alpha ) 
{ p.a /= alpha; p.b /= alpha;  p.c /= alpha; p.d /= alpha;  return p; }

template < class CT > 
inline quad3<CT> operator / ( const quad3<CT>& p, double alpha )
{ return quad3<CT>(p.a/alpha, p.b/alpha, p.c/alpha, p.d/alpha); }

template < class CT > 
inline quad3<CT>& operator /= ( quad3<CT>& p, double alpha ) 
{ p.a /= alpha; p.b /= alpha;  p.c /= alpha; p.d /= alpha;  return p; }

//========== unary minus
template < class CT > 
inline quad2<CT> operator - ( const quad2<CT>& p )
{ return quad2<CT>(-p.a, -p.b, -p.c, -p.d ); }

template < class CT > 
inline quad3<CT> operator - ( const quad3<CT>& p )
{ return quad3<CT>(-p.a, -p.b, -p.c, -p.d ); }

//========== plus minus point
//2d
template < class CT > 
inline quad2<CT> operator + ( const quad2<CT>& p, const pnt2<CT>& q )
{ return quad2<CT>(p.a+q, p.b+q, p.c+q, p.d+q); }

template < class CT > 
inline quad2<CT>& operator += ( quad2<CT>& p, const pnt2<CT>& add_q )
{ p.a += add_q; p.b += add_q; p.c += add_q; p.d += add_q; return p;}

template < class CT > 
inline quad2<CT> operator - ( const quad2<CT>& p, const pnt2<CT>& q )
{ return quad2<CT>(p.a-q, p.b-q, p.c-q, p.d-q); }

template < class CT > 
inline quad2<CT>& operator -= ( quad2<CT>& p, const pnt2<CT>& add_q )
{ p.a -= add_q; p.b -= add_q; p.c -= add_q; p.d -= add_q; return p; }
// 3d
template < class CT > 
inline quad3<CT> operator + ( const quad3<CT>& p, const pnt3<CT>& q )
{ return quad3<CT>(p.a+q, p.b+q, p.c+q, p.d+q); }

template < class CT > 
inline quad3<CT>& operator += ( quad3<CT>& p, const pnt3<CT>& add_q )
{ p.a += add_q; p.b += add_q; p.c += add_q; p.d += add_q; return p;}

template < class CT > 
inline quad3<CT> operator - ( const quad3<CT>& p, const pnt3<CT>& q )
{ return quad3<CT>(p.a-q, p.b-q, p.c-q, p.d-q); }

template < class CT > 
inline quad3<CT>& operator -= ( quad3<CT>& p, const pnt3<CT>& add_q )
{ p.a -= add_q; p.b -= add_q; p.c -= add_q; p.d -= add_q; return p; }

//=== misc

template < class CT > 
inline double perimeter( quad2<CT>& p )
{ return length( p.b-p.a)+length( p.c-p.b)+length( p.d-p.c)+length( p.a-p.d); }
template < class CT > 
inline double perimeter( quad3<CT>& p )
{ return length( p.b-p.a)+length( p.c-p.b)+length( p.d-p.c)+length( p.a-p.d); }

template < class CT > 
inline double area( quad2<CT>& p )
{ return length(cross(p.c - p.a, p.d - p.b)) / 2; }
template < class CT > 
inline double area( quad3<CT>& p )
{ return length(cross( p.c - p.a, p.d - p.b )) / 2; }


template < class CT >
inline quad2<CT>& invert_abcd2dcba( quad2<CT>& q ) // abcd --> dcba
{
  std::swap( q.a, q.d );
  std::swap( q.b, q.c );
  return q;
}

template < class CT >
inline quad2<CT>& invert_abcd2adcb( quad2<CT>& q ) // abcd --> adcb
{
  std::swap( q.d, q.b );
  return q;
}


template < class CT >
inline quad2<CT>& shift_abcd2bcda( quad2<CT>& q ) // abcd --> bcda
{
  pnt2<CT> t = q.a; q.a = q.b; q.b = q.c; q.c = q.d; q.d = t;
  return q;
}

template < class quad > 
double sides_ratio( quad& q )
{
  double len1 = length( q.b-q.a ) + length( q.c-q.d );
  double len2 = length( q.c-q.b ) + length( q.d-q.a );
  if (std::max( len1, len2 ) < 0.000000001)
    return 1.;
  return std::min( len1, len2 ) / std::max( len1, len2 );
}
//////
//////template < class CT > 
//////double sides_ratio( quad2< CT >& q )
//////{
//////  double len1 = length( q.b-q.a ) + length( q.c-q.d );
//////  double len2 = length( q.c-q.b ) + length( q.d-q.a );
//////  if (std::min( len1, len2 ) < 0.000000001)
//////    return 1.;
//////  return std::min( len1, len2 ) / std::max( len1, len2 );
//////}

template < class quad >
inline double max_cosangle( quad& q )
// returns [-1...1] ranged cosine of angle between a and b (or between b and a, symmetric)
{
  return max4( 
    cosangle( q.d-q.a, q.b-q.a ), 
    cosangle( q.a-q.b, q.c-q.b ),
    cosangle( q.b-q.c, q.d-q.c ),
    cosangle( q.c-q.d, q.a-q.d )
    );
}

template < class quad >
inline double min_cosangle( quad& q )
// returns [-1...1] ranged cosine of angle between a and b (or between b and a, symmetric)
{
  return min4( 
    cosangle( q.d-q.a, q.b-q.a ), 
    cosangle( q.a-q.b, q.c-q.b ),
    cosangle( q.b-q.c, q.d-q.c ),
    cosangle( q.c-q.d, q.a-q.d )
    );
}


}; // namespace se 

typedef se::quad2< int32_t > MinQuad2i;
typedef se::quad2< int16_t > MinQuad2s; 
typedef se::quad2< real32_t > MinQuad2f;
typedef se::quad2< real64_t > MinQuad2d;

typedef MinQuad2i MinQuad;

typedef se::quad3< int32_t > MinQuad3i;
typedef se::quad3< int16_t > MinQuad3s; 
typedef se::quad3< real32_t > MinQuad3f;
typedef se::quad3< real64_t > MinQuad3d;

#endif // __cplusplus

#endif // MINPOINT_H_INCLUDED
