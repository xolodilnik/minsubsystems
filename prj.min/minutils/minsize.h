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
#ifndef MINSIZE_H_INCLUDED
#define MINSIZE_H_INCLUDED

#include <minutils/mintyp.h>
#include <minutils/crossplat.h>

#ifndef __cplusplus

/**
 * @brief   Size of an image or a rectangle.
 * @details The struct @c MinSize represents a size of an image or a rectangle
 *          as a tuple (width, height), where width and height are integers.
 * @ingroup MinUtils_Geometry
 */
typedef struct
{
  int32_t width;    ///< The width of the rectangle or the image.
  int32_t height;   ///< The height of the rectangle or the image.
} MinSize;

typedef struct
{
  real32_t width;   ///< The width of the rectangle or the image.
  real32_t height;  ///< The height of the rectangle or the image.
} MinSize2f;

typedef struct
{
  int32_t width;    ///< The width of the rectangle or the image.
  int32_t height;   ///< The height of the rectangle or the image.
  int32_t depth;    ///< The depth of the rectangle or the image.
} MinSize3i;

typedef struct
{
  real32_t width;   ///< The width of the rectangle or the image.
  real32_t height;  ///< The height of the rectangle or the image.
  real32_t depth;   ///< The depth of the rectangle or the image.
} MinSize3i;

#endif  // __cplusplus

#ifdef __cplusplus

namespace se {
  template < class CT > // coords type
  struct size2
  {
    CT width, height;   // width and height values of the size
    size2( CT _width = 0, CT _height = 0 ) : width(_width), height(_height) {}
    template< class CT2 > 
    size2( const size2< CT2 >& size ): 
      width(static_cast<CT>(size.width)), height(static_cast<CT>(size.height)){}
  };

  template < class CT > // coords type
  struct size3
  {
    CT width, height, depth;   // width, height, and depth values of the size
    size3( CT _width = 0, CT _height = 0, CT _depth = 0 ) : 
      width(_width), height(_height), depth(_depth) {}
    template< class CT2 > 
    size3( const size3< CT2 >& size ): 
      width(static_cast<CT>(size.width)), 
      height(static_cast<CT>(size.height)),
      depth(static_cast<CT>(size.depth)){}
    template< class CT2 > 
    size3( const size2< CT2 >& size, CT2 _depth ):
      width(static_cast<CT>(size.width)),
      height(static_cast<CT>(size.height)),
      depth(static_cast<CT>(_depth)){}
  };

  template < class CT > 
  inline size2<CT> operator + ( const size2<CT>& p, const size2<CT>& q )
  { return size2<CT>(p.width+q.width, p.height+q.height); }

};  // namespace se

typedef se::size2< int32_t > MinSize2i;
typedef se::size3< int32_t > MinSize3i;
typedef se::size2< int16_t > MinSize2s;
typedef se::size3< int16_t > MinSize3s;
typedef se::size2< real32_t > MinSize2f;
typedef se::size3< real32_t > MinSize3f;
typedef se::size2< real64_t > MinSize2d;
typedef se::size3< real64_t > MinSize3d;

typedef MinSize2i MinSize;

#endif  // __cplusplus

/**
 * @brief   Inline constructors for @c MinSize data type.
 * @param   width  The width of the size object.
 * @param   height The height of the size object.
 * @returns A new @c MinSize object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinSize object using width and
 * height values.
 */
MUSTINLINE MinSize minSize(int32_t width, int32_t height)
{
#ifndef __cplusplus
  MinSize size = {width, height}; return size;
#else
  return MinSize(width, height);
#endif
}


#endif  // MINSIZE_H_INCLUDED
