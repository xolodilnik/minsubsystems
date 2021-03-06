include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/zlib
  ${CMAKE_CURRENT_BINARY_DIR}/libtiff
  ${CMAKE_CURRENT_BINARY_DIR}/libjpeg
  ${CMAKE_CURRENT_BINARY_DIR}/libpng
  ${CMAKE_CURRENT_BINARY_DIR}/libwebp
)

add_subdirectory(zlib)
set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
set(ZLIB_LIBRARIES zlib)
set(ZLIB_FOUND TRUE)

add_subdirectory(libjpeg)
set(LIBJPEG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/jpeg)
set(LIBJPEG_LIBRARIES jpeg)
set(LIBJPEG_FOUND TRUE)

if (WITH_TIFF)
  add_subdirectory(libtiff)
  set(TIFF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tiff)
  set(TIFF_LIBRARIES tiff)
  set(TIFF_FOUND TRUE)
endif (WITH_TIFF)

IF (WITH_WEBP)
  ADD_SUBDIRECTORY(libwebp)
ENDIF (WITH_WEBP)

add_subdirectory(libpng)
set(PNG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/png)
set(PNG_LIBRARIES png)
set(PNG_LIBRARY png)
set(PNG_FOUND TRUE)

if (WITH_TESTS)
  add_subdirectory(gtest)
endif (WITH_TESTS)

#ADD_SUBDIRECTORY(tbb)
#ADD_SUBDIRECTORY(protobuf)
ADD_SUBDIRECTORY(json-cpp)
