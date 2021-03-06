## Copyright (c) 2013, Visillect Service LLC. All rights reserved.
## 
## Redistribution and use in source and binary forms, with or without modification,
## are permitted provided that the following conditions are met:
## 
##    1. Redistributions of source code must retain the above copyright notice,
##       this list of conditions and the following disclaimer.
## 
##    2. Redistributions in binary form must reproduce the above copyright notice,
##       this list of conditions and the following disclaimer in the documentation
##       and/or other materials provided with the distribution.
## 
## THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS "AS IS" AND ANY EXPRESS OR
## IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
## MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
## SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
## INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
## LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
## LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
## OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
## ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
## 
## The views and conclusions contained in the software and documentation are those
## of the authors and should not be interpreted as representing official policies,
## either expressed or implied, of copyright holders.


cmake_minimum_required(VERSION 2.8)

# remove unused configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible build types" FORCE)

# IMPORTANT: this command must be placed after CMAKE_CONFIGURATION_TYPES is changed
project(minsubsystems)

# CMAKE_BUILD_TYPE must not be used with non make-based generators (we only consider MSVC here)
# FIXME: we need a generic way of checking whether a generator is make-based or not
if(MSVC_IDE)
  if(CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "CMAKE_BUILD_TYPE is not supported by MSVC generator")
  endif()
else()
  if(CMAKE_BUILD_TYPE)
    if(NOT(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release"))
      message(FATAL_ERROR "Invalid CMAKE_BUILD_TYPE. Possible values: Release [default], Debug")
    endif()
  else()
    message(STATUS "Using default build type: Release")
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

if(MSVC)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif(MSVC)

include(cmake/constants.cmake)
include(cmake/func_macro.cmake)
include(cmake/compiler_definitions.cmake)
include(cmake/options.cmake)

# boost setup
find_package(Boost REQUIRED)
#set(MINSUBSYSTEMS_BOOST_VERSION "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}")
#if(MINSUBSYSTEMS_BOOST_VERSION VERSION_LESS "1.55")
#  message(FATAL_ERROR "Bad boost version.")
#endif()
#include_directories(${Boost_INCLUDE_DIRS})

# global include directories
include_directories(include)
include_directories(prj.min)
include_directories(prj.thirdparty)

# Sort of ad-hoc include paths.
include_directories(prj.thirdparty/zlib/src)

# output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${MINSUBSYSTEMS_ROOT}/bin.${MINSUBSYSTEMS_ARCH}.debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${MINSUBSYSTEMS_ROOT}/bin.${MINSUBSYSTEMS_ARCH}.release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${MINSUBSYSTEMS_ROOT}/lib.${MINSUBSYSTEMS_ARCH}.debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${MINSUBSYSTEMS_ROOT}/lib.${MINSUBSYSTEMS_ARCH}.release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${MINSUBSYSTEMS_ROOT}/lib.${MINSUBSYSTEMS_ARCH}.debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${MINSUBSYSTEMS_ROOT}/lib.${MINSUBSYSTEMS_ARCH}.release)

add_subdirectory(prj.min)
add_subdirectory(prj.thirdparty)