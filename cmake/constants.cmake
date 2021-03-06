# --- architecture ---
if(WIN32)
  set(MINSUBSYSTEMS_OPERATING_SYSTEM "win")
elseif(UNIX)
  set(MINSUBSYSTEMS_OPERATING_SYSTEM "linux")
else()
  message(FATAL_ERROR "Unknown operating system")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL "4")
  set(MINSUBSYSTEMS_CPU_BITNESS "32")
elseif(CMAKE_SIZEOF_VOID_P EQUAL "8")
  set(MINSUBSYSTEMS_CPU_BITNESS "64")
else()
  message(FATAL_ERROR "Unsupported CPU bitness")
endif()

set(MINSUBSYSTEMS_ARCH ${MINSUBSYSTEMS_OPERATING_SYSTEM}${MINSUBSYSTEMS_CPU_BITNESS})
message(STATUS "Target architecture: ${MINSUBSYSTEMS_ARCH}")

# --- directories ---
set(MINSUBSYSTEMS_ROOT ${minsubsystems_SOURCE_DIR})
set(MINSUBSYSTEMS_INCLUDE_ROOT ${MINSUBSYSTEMS_ROOT}/include)
set(MINSUBSYSTEMS_LIB_ROOT ${MINSUBSYSTEMS_ROOT}/lib/${MINSUBSYSTEMS_ARCH})
if(WIN32 AND NOT MSVC_IDE)
  set(MINSUBSYSTEMS_LIB_ROOT ${MINSUBSYSTEMS_LIB_ROOT} ${MINSUBSYSTEMS_LIB_ROOT}/${CMAKE_BUILD_TYPE})
endif()

