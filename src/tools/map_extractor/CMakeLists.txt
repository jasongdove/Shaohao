# This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB_RECURSE sources *.cpp *.h)

include_directories (
    PRIVATE
      ${CMAKE_SOURCE_DIR}/src/common
      ${CMAKE_SOURCE_DIR}/src/common/Utilities
      ${CMAKE_SOURCE_DIR}/src/common/Debugging
      ${CMAKE_SOURCE_DIR}/src/common/Collision/Maps
      ${CMAKE_SOURCE_DIR}/dep/StormLib/src
      ${CMAKE_SOURCE_DIR}/dep/g3dlite/source
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/loadlib
)

add_executable(mapextractor
  ${sources}
)

target_link_libraries(mapextractor
    PRIVATE
      common
      bzip2
      zlib
      storm
      g3dlib
      ${CMAKE_DL_LIBS}
)

add_dependencies(mapextractor storm)

if( UNIX )
  install(TARGETS mapextractor DESTINATION bin)
elseif( WIN32 )
  install(TARGETS mapextractor DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()