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
#ifndef MINRECT_H_INCLUDED
#define MINRECT_H_INCLUDED

#include <algorithm>

#include <minutils/mintyp.h>
#include <minutils/minpoint.h>
#include <minutils/crossplat.h>

#ifndef __cplusplus
/**
 * @brief   2D rectangle <x,y,width,height> with integer/real coordinates.
 * @details @c MinRect represents a 2D rectangle where x,y,width,height are 32 bit integers.
 *          @c MinRect2f represents a 2D rectangle where x,y,width,height are 32 bit float.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  int32_t x; ///< The x-coordinate of the left-top corner point
  int32_t y; ///< The y-coordinate of the left-top corner point
  int32_t width;  ///< The width (x-size) of the rectangle
  int32_t height; ///< The height (y-size) of the rectangle
} MinRect;

typedef struct
{
  real32_t x; ///< The x-coordinate of the left-top corner point
  real32_t y; ///< The y-coordinate of the left-top corner point
  real32_t width;  ///< The width (x-size) of the rectangle
  real32_t height; ///< The height (y-size) of the rectangle
} MinRect2f;


/**
 * @brief   3D rectangle with real coordinates (zero-based).
 * @details The structure @c MinRect3f represents a 3D rectangle 
 *          (parallelepiped) as  (x, y, z, width, height, depth),
 *          where x, y, z, width, height, depth are real.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  real32_t x; ///< The x-coordinate of the left-top corner point.
  real32_t y; ///< The y-coordinate of the left-top corner point.
  real32_t z; ///< The z-coordinate of the left-top corner point.
  real32_t width;  ///< The width (x-size) of the rectangle
  real32_t height; ///< The height (y-size) of the rectangle
  real32_t depth;  ///< The depth (z-size) of the rectangle
} MinRect3f;

#endif  // !__cplusplus

#ifdef __cplusplus
namespace se {

template < class CT > // coords type
struct rect2 // 2d parallelepiped coaxial to XY axes
{
  CT x,y; ///< x,y - coordinates of the left-top corner
  CT width,height; ///< width,height - x size and y size
  rect2( CT _x=0, CT _y=0, CT _width=0, CT _height=0 ): x(_x), y(_y), width(_width), height(_height)
  {}
  template< class CT2 > 
  rect2( const rect2< CT2 >& rc ):
    x(static_cast<CT>(rc.x)),
    y(static_cast<CT>(rc.y)),
    width(static_cast<CT>(rc.width)),
    height(static_cast<CT>(rc.height))
    {}
};

template < class CT > // coords type
struct rect3 // 3d parallelepiped coaxial to XYZ axes
{
  CT x,y,z; ///< x,y,z - coordinates of the corner (usually with min values)
  CT width,height,depth; ///< width,height,depth - x size, y size, z size respectively
  rect3( CT _x=0, CT _y=0, CT _z=0, CT width=0, CT height=0, CT depth=0  ):
    x(_x),y(_y),z(_z),width(width),height(height),depth(depth){}

  template< class CT2 > 
  rect3( const rect3< CT2 >& rc ):
    x(static_cast<CT>(rc.x)),
    y(static_cast<CT>(rc.y)),
    z(static_cast<CT>(rc.z)),
    width(static_cast<CT>(rc.width)),
    height(static_cast<CT>(rc.height)),
    depth(static_cast<CT>(rc.depth))
    {}
};

template < class CT > 
inline rect2<CT> operator * ( const rect2<CT>& p, double alpha ) 
{ return rect2<CT>(static_cast<CT>(p.x*alpha), static_cast<CT>(p.y*alpha),
                   static_cast<CT>(p.width*alpha), static_cast<CT>(p.height*alpha) );}

template < class CT > 
inline rect2<CT> operator / ( const rect2<CT>& p, double alpha )
{ return rect2<CT>(p.x/alpha, p.y/alpha, p.width/alpha, p.height/alpha ); }

template < class CT > 
inline rect3<CT> operator * ( const rect3<CT>& p, double alpha ) 
{ return rect3<CT>(p.x*alpha, p.y*alpha, p.z*alpha,
                   p.width*alpha, p.height*alpha, p.depth*alpha );}

template < class CT > 
inline rect3<CT> operator / ( const rect3<CT>& p, double alpha )
{ return rect3<CT>(p.x/alpha, p.y/alpha, p.z/alpha, 
                   p.width/alpha, p.height/alpha, p.depth/alpha ); }

template < class CT >
inline rect2<CT >& inflate( rect2<CT>& r, double delta ) // inflate rectangle (or deflate with negative delta)
{
  r.x -= delta;  r.y -= delta; 
  r.height += 2*delta; r.width += 2*delta;
  return r;
}

template < class CT >
inline rect3<CT >& inflate( rect3<CT>& r, double delta ) // inflate rectangle (or deflate with negative delta)
{
  r.x -= delta;  r.y -= delta;   r.z -= delta;
  r.height += 2*delta; r.width += 2*delta;  r.depth += 2*delta;
  return r;
}

template < class CT >
inline CT area(const rect2<CT> &r) {
  if (r.width <= 0 || r.height <= 0)
     return 0;
  return r.width * r.height;
}

//========== plus minus point
//2d
template < class CT > 
inline rect2<CT> operator + ( const rect2<CT>& r, const pnt2<CT>& p )
{ return rect2<CT>(r.x+p.x, r.y+p.y, r.width, r.height); }

template < class CT > 
inline rect2<CT>& operator += ( const rect2<CT>& r, const pnt2<CT>& p )
{ r.x += p.x; r.y += p.y; }

template < class CT > 
inline rect2<CT> operator - ( const rect2<CT>& r, const pnt2<CT>& p )
{ return rect2<CT>(r.x-p.x, r.y-p.y, r.width, r.height); }

template < class CT > 
inline rect2<CT>& operator -= ( const rect2<CT>& r, const pnt2<CT>& p )
{ r.x -= p.x; r.y -= p.y; }
// 3d
template < class CT > 
inline rect3<CT> operator + ( const rect3<CT>& r, const pnt3<CT>& p )
{ return rect3<CT>(r.x+p.x, r.y+p.y, r.z+p.z, r.width, r.height, r.depth); }

template < class CT > 
inline rect3<CT>& operator += ( const rect3<CT>& r, const pnt3<CT>& p )
{ r.x += p.x; r.y += p.y; r.z += p.z; }

template < class CT > 
inline rect3<CT> operator - ( const rect3<CT>& r, const pnt3<CT>& p )
{ return rect3<CT>(r.x-p.x, r.y-p.y, r.z-p.z, r.width, r.height, r.depth); }

template < class CT > 
inline rect3<CT>& operator -= ( const rect3<CT>& r, const pnt3<CT>& p )
{ r.x -= p.x; r.y -= p.y; r.z -= p.z; }

}; // namespace se 

typedef se::rect2< int32_t > MinRect2i;
typedef se::rect3< int32_t > MinRect3i;
typedef se::rect2< int16_t > MinRect2s;
typedef se::rect3< int16_t > MinRect3s;
typedef se::rect2< real32_t > MinRect2f;
typedef se::rect3< real32_t > MinRect3f;
typedef se::rect2< real64_t > MinRect2d;
typedef se::rect3< real64_t > MinRect3d;

typedef MinRect2i MinRect;

#endif // __cplusplus


/**
 * @brief   Inline constructors for @c MinRect data type.
 * @param   x      The x-coordinate of the top-left corner.
 * @param   y      The x-coordinate of the top-left corner.
 * @param   width  The width of the rectangle.
 * @param   height The height of the rectangle.
 * @returns A new @c MinRect object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinRect object using x-coordinate,
 * y-coordinate, width, and height values.
 */
MUSTINLINE MinRect minRect(int32_t x, int32_t y,
                                  int32_t width, int32_t height)
{
  MinRect rect( x, y, width, height );
  return rect;
}


/**
 * @brief   Returns minimal rectangle which contains @first and @second
 * @param   first  The first rectangle
 * @param   second The second rectangle
 * @returns A new @c MinRect object which contains both @first and
            @second rectangles.
 * @ingroup MinUtils_Geometry
 */

inline MinRect MergeRects(MinRect first, MinRect second)
{
  MinRect dst = first;
  int right = std::max<int32_t>(dst.x + dst.width, second.x + second.width);
  int bottom = std::max<int32_t>(dst.y + dst.height, second.y + second.height);
  dst.x = std::min<int32_t>(dst.x, second.x);
  dst.y = std::min<int32_t>(dst.y, second.y);
  dst.width = right - dst.x;
  dst.height = bottom - dst.y;
  return dst;
}

/**
 * @brief   Returns @first and @second rects intersection
 * @param   first  The first rectangle
 * @param   second The second rectangle
 * @returns A new @c MinRect object which is an intersection of @first and @second
 * @ingroup MinUtils_Geometry
 */
inline MinRect IntersectRects(MinRect first, MinRect second)
{
  MinRect dst = first;
  int right = std::min<int32_t>(dst.x + dst.width, second.x + second.width);
  int bottom = std::min<int32_t>(dst.y + dst.height, second.y + second.height);
  dst.x = std::max<int32_t>(dst.x, second.x);
  dst.y = std::max<int32_t>(dst.y, second.y);
  dst.width = right - dst.x;
  dst.height = bottom - dst.y;
  if (dst.width <= 0 || dst.height <= 0)
  {
    dst = MinRect();
  }
  return dst;
}

#endif // MINRECT_H_INCLUDED
