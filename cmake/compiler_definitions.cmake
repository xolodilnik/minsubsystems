# Only option-independent compiler definition are set here.
# Option-specific definitions reside in options.cmake.

# ------------- COMPILER SPECIFIC -----------
if(MSVC)
  # suppress some Visual Studio warnings
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)

  # allow SSE
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")

  # use assembler only on MSVC 32 bit compilator
  if(NOT CMAKE_CL_64)
    add_definitions(-DASSEMBLER_ENABLED)
    message(STATUS "Assembler enabled")
  else()
    message(STATUS "Assembler disabled")
  endif()
endif()

set(BUILD_GCC_ARCH "native" CACHE STRING "gcc -march and -mtune value")
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  message(STATUS "Using compiler flags -march=${BUILD_GCC_ARCH} and -tune=${BUILD_GCC_ARCH}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=${BUILD_GCC_ARCH} -mtune=${BUILD_GCC_ARCH} -fPIC -fno-omit-frame-pointer -Wno-attributes")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${BUILD_GCC_ARCH} -mtune=${BUILD_GCC_ARCH} -fPIC -fno-omit-frame-pointer -Wno-attributes")

  add_definitions(-DCOMFORT_GNU_COMPILER)

  # Strange, but this fixes some troubles with undefined references.
  # TODO: investigate issues
  # (like libttrvpsubsystems.so: undefined reference to `avcodec_decode_video')
  add_definitions(-Wl,--no-undefined)
endif()
