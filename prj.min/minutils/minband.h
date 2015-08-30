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
#ifndef MINBAND_H_INCLUDED
#define MINBAND_H_INCLUDED

#include <minutils/mintyp.h>
#include <minutils/minpoint.h>
#include <minutils/crossplat.h>

/**
 * @brief   Band object represented as two ends points and band width.
 * @details The struct @c MinBand represents a band object as a tuple of two
 *          MinPoint's and band width.
 * @ingroup MinUtils_Geometry
 */

#ifndef __cplusplus
typedef struct
{
  MinPoint u;   ///< First point.
  MinPoint v;   ///< Second point.
  int32_t  w;   ///< Band width.
} MinBand;

typedef struct
{
  MinPoint2f u;   ///< First point
  MinPoint2f v;   ///< Second point
  real32_t   w;   ///< Band width
} MinBand2f;

#endif  // __cplusplus

#ifdef __cplusplus
namespace se {
  template < class CT >
  struct bnd2
  {
    se::pnt2<CT> u; ///< First point
    se::pnt2<CT> v; ///< Second point
    CT           w; ///< Band width
    bnd2(const se::pnt2<CT>& _u, const se::pnt2<CT>& _v, CT _w) :
      u(_u), v(_v), w(_w) {};
    bnd2(const CT _xB, const CT _yB, const CT _xE, const CT _yE, const CT _w) :
      u(pnt2<CT>(_xB, _yB)), v(pnt2<CT>(_xE, _yE)), w(_w) {};
    bnd2() :w(0) {};
  };
};  // namespace se

typedef se::bnd2< int32_t > MinBand2i;
typedef se::bnd2< int16_t > MinBand2s;
typedef se::bnd2< real32_t > MinBand2f;
typedef se::bnd2< real64_t > MinBand2d;

typedef MinBand2i MinBand;

#endif  // __cplusplus


/**
 * @brief   Inline constructor for @c MinBand data type.
 * @param   u      First point.
 * @param   v      Second point.
 * @param   w      Band width.
 * @returns A new @c MinBand object.
 * @ingroup MinUtils_Geometry
 *
 * The function constructs a new @c MinBand object using two points and width.
 */
MUSTINLINE MinBand minBand(MinPoint u, MinPoint v, int w)
{
#ifndef __cplusplus
  MinBand band = {u, v, w};  return band;
#else
  return MinBand(u, v, w);
#endif
}

#endif  // MINBAND_H_INCLUDED
