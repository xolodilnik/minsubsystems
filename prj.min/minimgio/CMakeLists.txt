project(minimgio)

option(WITH_DSHOW "Use DirectX Show" OFF)

set(minimgio_public_HEADERS
  define.h
  device.h
  contrib.h
  minimgio.h)

set(minimgio_HEADERS
  ${minimgio_public_HEADERS}
  src/minimgiolst.h
  src/minimgiopng.h
  src/minimgiojpeg.h
  src/minimgiotiff.h
  src/minimgiowebp.h
  src/minimgiodevice.h
  src/subsystem.h
  src/camstream.h
  src/stream.h
  src/utils.h
  src/pack.h)

set(minimgio_SRCS
  src/minimgio.cpp
  src/device.cpp
  src/contrib.cpp
  src/minimgiolst.cpp
  src/minimgiopng.cpp
  src/minimgiojpeg.cpp
  src/minimgiotiff.cpp
  src/minimgiowebp.cpp
  src/minimgiodevice.cpp
  src/subsystem.cpp
  src/camstream.cpp
  src/utils.cpp
  src/pack.cpp)

set(thirdparty_LIBS)

option(WITH_JPEG "Turns on support for JPEG image format." ON)
if (WITH_JPEG)
  add_definitions(-DWITH_JPEG)
  set(thirdparty_LIBS ${thirdparty_LIBS} jpeg)
endif (WITH_JPEG)

option(WITH_TIFF "Turns on support for TIFF image format." ON)
if (WITH_TIFF)
  add_definitions(-DWITH_TIFF)
  set(thirdparty_LIBS ${thirdparty_LIBS} tiff)
  include_directories(${CMAKE_BINARY_DIR}/prj.thirdparty/libtiff)
endif (WITH_TIFF)

option(WITH_PNG "Turns on support for PNG image format." ON)
if (WITH_PNG)
  add_definitions(-DWITH_PNG)
  set(thirdparty_LIBS ${thirdparty_LIBS} png)
  include_directories(${CMAKE_BINARY_DIR}/prj.thirdparty/libpng)
endif (WITH_PNG)

if (WITH_TIFF OR WITH_PNG)
  set(thirdparty_LIBS ${thirdparty_LIBS} zlib)
endif()

option(WITH_WEBP "Turns on support for WebP image format." OFF)
if (WITH_WEBP)
  add_definitions(-DWITH_WEBP)
  set(thirdparty_LIBS ${thirdparty_LIBS} libwebp)
  include_directories(${CMAKE_SOURCE_DIR}/prj.thirdparty/libwebp)
endif (WITH_WEBP)

if(WITH_DSHOW)

  add_definitions(-DDIRECTX_SHOW)
  
  set(minimgio_HEADERS 
    ${minimgio_HEADERS}
    src/dshowutils.h
    src/dshowdevice.h
    src/dshowsubsystem.h)

  set(minimgio_SRCS
    ${minimgio_SRCS}
    src/dshowutils.cpp
    src/dshowdevice.cpp
    src/dshowsubsystem.cpp)

  set(thirdparty_LIBS
    ${thirdparty_LIBS}
    winmm
    dsbase
    strmiids)

  include_directories(
    ${CMAKE_SOURCE_DIR}/prj.thirdparty/dsbase)

elseif(WITH_AVFOUNDATION)

  add_definitions(-DAVFOUNDATION)

  set(av_HEADERS 
    src/avfoundationdevice.h
    src/avfoundationsubsystem.h
    src/avfoundationutils.h)
  set(minimgio_HEADERS
    ${minimgio_HEADERS} 
    ${av_HEADERS})

  set(av_SRCS
    src/avfoundationdevice.mm
    src/avfoundationsubsystem.mm)
  set(minimgio_SRCS
    ${minimgio_SRCS} 
    ${av_SRCS})

endif()

if(BUILD_SHARED_LIBS)
  add_definitions(-DMINIMGIO_EXPORTS)
endif()

add_library(minimgio ${minimgio_SRCS} ${minimgio_HEADERS})

# target_link_libraries can't take empty argument - so we check
if(thirdparty_LIBS)
   target_link_libraries(minimgio ${thirdparty_LIBS})
   add_dependencies(minimgio ${thirdparty_LIBS})
endif()   

if(BUILD_GO)
  add_executable(minimgiodev_go src/minimgiodev_go.cpp)
  target_link_libraries(minimgiodev_go minimgio minimgapi)
  add_dependencies(minimgiodev_go minimgio minimgapi)

  if(WITH_AVFOUNDATION)
    macro(ADD_FRAMEWORK appname fwname)
    find_library(FRAMEWORK_${fwname}
        NAMES ${fwname}
        PATHS ${CMAKE_OSX_SYSROOT}/System/Library
        PATH_SUFFIXES Frameworks
        NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
    endmacro(ADD_FRAMEWORK)

    # SET (CMAKE_EXE_LINKER_FLAGS
    # "-framework CoreMedia -framework CoreVideo -framework AVFoundation -framework Foundation")

    add_framework(minimgiodev_go CoreMedia)
    add_framework(minimgiodev_go CoreVideo)
    add_framework(minimgiodev_go Foundation)
    add_framework(minimgiodev_go AVFoundation)
   # add_framework(minimgiodev_go CoreGraphics)
   # add_framework(minimgiodev_go MobileCoreServices)
   # add_framework(minimgiodev_go ImageIO)

  endif(WITH_AVFOUNDATION)

endif(BUILD_GO)

if(minimgio_INSTALL_SDK)

  install(FILES ${minimgio_public_HEADERS}
    DESTINATION include/minimgio)
  install(TARGETS minimgio
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

elseif(minimgio_INSTALL_BINARY AND BUILD_SHARED_LIBS)

  install(TARGETS minimgio
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib)

endif()
