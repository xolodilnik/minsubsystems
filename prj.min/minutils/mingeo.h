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

/**
 * @file   mingeo.h
 * @brief  Definition of basic geometry data structures.
 */

#pragma once
#ifndef MINGEO_H_INCLUDED
#define MINGEO_H_INCLUDED

#define _USE_MATH_DEFINES
#include <cmath>

#include <minutils/mintyp.h>
#include <minutils/crossplat.h>

/**
 * @defgroup MinUtils_Geometry Basic Geometry Data Structures
 * @brief    The module specifies basic geometry data structures and auxiliary
 *           inline functions using for convenient construction of them.
 *           Types defined for integer and real coordinates, 
 *           We use following suffixes for coordinate types: 
 *           i - int32; s - int16; f - float32; d - float64
 *           
 */

#include <minutils/minpoint.h> ///<< MinPoint2[i|s|f|d] and MinPoint3[i|s|f|d]
#include <minutils/minsize.h>  ///<< MinSize2[i|s|f|d] and MinSize3[i|s|f|d]
#include <minutils/minrect.h>  ///<< MinRect2[i|s|f|d] and MinRect3[i|s|f|d]
#include <minutils/minsegm.h>  ///<< MinSegm2[i|s|f|d] and MinSegm3[i|s|f|d]
#include <minutils/minband.h>  ///<< MinBand2[i|s|f|d]
#include <minutils/minquad.h>  ///<< MinQuad2[i|s|f|d] and MinQuad3[i|s|f|d]

namespace se {

template < class CT > 
inline bool inside( CT a, CT x, CT b )  // a <= x <= b
{ return a <= x && x <= b; }

template < class CT > 
inline bool inside( rect2<CT>& rc, pnt2< CT > p ) // point inside rectangle 
{ return  inside( rc.x , p.x, rc.x + rc.width ) && 
          inside( rc.y , p.y, rc.y + rc.height );
}

template < class CT > 
inline bool inside( rect3<CT>& rc, pnt3< CT > p ) // point inside rectangle 3d 
{ return  inside( rc.x , p.x, rc.x + rc.width ) && 
          inside( rc.y , p.y, rc.y + rc.height ) &&
          inside( rc.z , p.z, rc.z + rc.depth );
}

template < class CT > 
inline bool inside( rect2<CT>& rc, segm2< CT > s ) // segment inside rectangle
{ return  inside( rc, s.a ) && inside( rc, s.b ); }

template < class CT > 
inline bool inside( rect3<CT>& rc, segm3< CT > s ) // segment inside rectangle
{ return  inside( rc, s.a ) && inside( rc, s.b ); }

}; //se

#endif  // MINGEO_H_INCLUDED
