
# --- gprof support ---
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  option(ENABLE_GPROF "Compile with gprof support" OFF)
  if (ENABLE_GPROF)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -pg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pg")
  endif()
endif()

# Enables unit tests.
# This requires gtest library attached to the build.
OPTION(WITH_TESTS "Turns tests on if enabled." OFF)
IF (WITH_TESTS)
  MESSAGE(STATUS "Test programs are enabled")
ENDIF (WITH_TESTS)

OPTION(WITH_DEMOS "Turns demos on if enabled." OFF)
IF (WITH_DEMOS)
  MESSAGE(STATUS "Demo programs are enabled")
ENDIF (WITH_DEMOS)

# Enables usage of Single Instruction Multiple Data instruction sets
# of the processor, if available.
OPTION(WITH_SIMD "Use SIMD intrinsics." ON)
STRING(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} SYSTEM_PROCESSOR)
MESSAGE(STATUS "System processor (tolower) is ${SYSTEM_PROCESSOR}")
IF (WITH_SIMD)
  IF (${SYSTEM_PROCESSOR} MATCHES "^(i.86|x86|x86_64)$" OR
      ${SYSTEM_PROCESSOR} MATCHES "amd.*64")
    MESSAGE(STATUS "Intel SSE intrinsics are enabled.")
    ADD_DEFINITIONS(-DUSE_SSE_SIMD)

    IF (CMAKE_COMPILER_IS_GNUCXX OR
        CMAKE_COMPILER_IS_GNUC OR
        ${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse -msse2")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2")
    ELSEIF(MSVC)
      ADD_DEFINITIONS(/arch:SSE2)
    ENDIF()
  ENDIF()
  
  IF (${SYSTEM_PROCESSOR} MATCHES "^arm.*$")
    MESSAGE(STATUS "ARM NEON intrinsics are enabled.")
    ADD_DEFINITIONS(-DUSE_NEON_SIMD)
    IF (CMAKE_COMPILER_IS_GNUCXX OR
        CMAKE_COMPILER_IS_GNUC OR
        ${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
    ENDIF()
  ENDIF()
ENDIF()

# --- debug info in release build ---
if(WIN32)
  set(RELEASE_WITH_DEBUG_INFO_DEFAULT OFF)
else()
  set(RELEASE_WITH_DEBUG_INFO_DEFAULT ON)
endif()
option(RELEASE_WITH_DEBUG_INFO "Build release with debug info" ${RELEASE_WITH_DEBUG_INFO_DEFAULT})
if(RELEASE_WITH_DEBUG_INFO)
  message(STATUS "Release will be built with debug info")
  if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -g")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g")
  elseif(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /DEBUG")
  endif()
endif()
