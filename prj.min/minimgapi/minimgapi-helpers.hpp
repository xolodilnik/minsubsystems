/*
Copyright (c) 2011-2013, Smart Engines Limited. All rights reserved.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS "AS IS" AND ANY EXPRESS OR
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
 * @file   minimgapi-helpers.hpp
 * @brief  MinImgAPI c++ helpers interface.
 */

#pragma once
#ifndef MINIMGAPI_HELPERS_HPP_INCLUDED
#define MINIMGAPI_HELPERS_HPP_INCLUDED

#include <minutils/minerr.h>
#include <minutils/crossplat.h>
#include <minimgapi/minimgapi.h>
#include <minimgapi/minimgapi-inl.h>

template<typename T> MUSTINLINE MinFmt GetMinFmtByCType();
template<> MinFmt MUSTINLINE GetMinFmtByCType<uint8_t>()  { return FMT_UINT; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<uint16_t>() { return FMT_UINT; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<uint32_t>() { return FMT_UINT; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<uint64_t>() { return FMT_UINT; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<int8_t>()   { return FMT_INT;  }
template<> MinFmt MUSTINLINE GetMinFmtByCType<int16_t>()  { return FMT_INT;  }
template<> MinFmt MUSTINLINE GetMinFmtByCType<int32_t>()  { return FMT_INT;  }
template<> MinFmt MUSTINLINE GetMinFmtByCType<int64_t>()  { return FMT_INT;  }
template<> MinFmt MUSTINLINE GetMinFmtByCType<real16_t>() { return FMT_REAL; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<real32_t>() { return FMT_REAL; }
template<> MinFmt MUSTINLINE GetMinFmtByCType<real64_t>() { return FMT_REAL; }

template<typename T> MUSTINLINE MinTyp GetMinTypByCType() {
  return _GetTypByFmtAndDepth(GetMinFmtByCType<T>(), sizeof(T));
}
template<> MUSTINLINE MinTyp GetMinTypByCType<bool>() {
  return TYP_UINT1;
}

#endif // MINIMGAPI_HELPERS_HPP_INCLUDED
