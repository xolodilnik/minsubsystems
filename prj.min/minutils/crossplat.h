/*

Copyright (c) 2011, Smart Engines Limited. All rights reserved. 

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of
      conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list
      of conditions and the following disclaimer in the documentation and/or other materials
      provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of copyright holders.

*/

/**
 * @file   crossplat.h
 * @brief  Different cross-platform declarations.
*/

#ifndef CROSSPLAT_H_INCLUDED
#define CROSSPLAT_H_INCLUDED

/**
 * @defgroup MinUtils_Crossplat Cross-platform Declarations
 * @brief    The module contains macros and functions which provides
 *           cross-platform compiling of the library.
 */

/**
 * @def     MUSTINLINE
 * @brief   Specifiers a cross-platform instruct the compiler to insert a copy 
 *          of the function body into each place the function is called
 * @ingroup MinUtils_Crossplat
 */

/**
 * @def     STATIC_SPECIAL
 * @brief   On MSVC a specialization of template function does not inherit
 *          storage class. So one has to set storage class manually which is
 *          forbidden by C++ Standard and is not compileable on g++.
 * @ingroup MinUtils_Crossplat
 */

/**
 * @def     alignedmalloc(size, alignment)
 * @brief   Specifies cross-platform aligned allocation.
 * @ingroup MinUtils_Crossplat
 */

/**
 * @def     alignedfree(ptr)
 * @brief   Specifies cross-platform aligned deallocations.
 * @ingroup MinUtils_Crossplat
 */


#ifdef _MSC_VER
/**
 * @def     snprintf
 * @brief   Defines @c snprintf for use with MSVC.
 * @ingroup MinUtils_Crossplat
 */
#endif

#ifndef NO_INLINE
#  ifndef MUSTINLINE
#    ifdef _MSC_VER 
#      define MUSTINLINE inline __forceinline
#    else
#      define MUSTINLINE inline __attribute__((always_inline))
#    endif
#  endif // MUSTINLINE
#else
# undef MUSTINLINE
# define MUSTINLINE
#endif // NO_INLINE

#ifndef STATIC_SPECIAL
#  if !defined(__clang__) && (defined(_MSC_VER) || (__GNUC__ < 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ < 3)))
#    define STATIC_SPECIAL static
#  else
#    define STATIC_SPECIAL
#  endif
#endif // STATIC_SPECIAL

#if defined(__MINGW32__)
#  include <malloc.h>
#  define alignedmalloc(size, alignment) __mingw_aligned_malloc(size, alignment)
#  define alignedfree(ptr)               __mingw_aligned_free(ptr)
#elif defined(USE_SSE_SIMD)
#  include <emmintrin.h>
#  define alignedmalloc(size, alignment) _mm_malloc(size, alignment)
#  define alignedfree(ptr)               _mm_free(ptr)
#else
#  include <stdlib.h>
#  define alignedmalloc(size, alignment) malloc(size)
#  define alignedfree(ptr)               free(ptr)
#endif

#if defined(__GNUC__)
#  define MIN_HAS_BUILTIN_PREFETCH
#elif defined(__clang__)
#  if defined(__has_builtin) && __has_builtin(__builtin_prefetch)
#    define MIN_HAS_BUILTIN_PREFETCH
#  endif
#endif

#ifdef MIN_HAS_BUILTIN_PREFETCH
#  define MIN_PREFETCH(ADDR) __builtin_prefetch(ADDR,0)
#else
#  define MIN_PREFETCH(ADDR)
#endif

#ifdef _MSC_VER
#  define snprintf _snprintf
#endif

#ifdef _MSC_VER
#  define MIN_FUNCNAME_MACRO __FUNCSIG__
#else // Assume GCC-compatible compiler here.
#  define MIN_FUNCNAME_MACRO __PRETTY_FUNCTION__
#endif // _MSC_VER

/**
 * @def     IS_BY_DEFAULT(a)
 * @brief   Specifies a default value for a parameter of a function.
 * @ingroup MinUtils_Crossplat
 */

#ifdef __cplusplus
#  define IS_BY_DEFAULT(a) = a
#else 
#  define IS_BY_DEFAULT(a)
#endif

#endif // CROSSPLAT_H_INCLUDED
