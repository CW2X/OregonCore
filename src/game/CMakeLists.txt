# This file is part of the MaNGOSCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(LIBRARY_NAME game)

add_definitions(-DDT_POLYREF64)
add_definitions(-D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0)

# Find all the input files
FILE(GLOB_RECURSE LIBRARY_SRCS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*.h" "*.cpp")

set(PCH_BASE_FILENAME "pchdef")
# exclude pchdef files
set (EXCLUDE_FILE "${PCH_BASE_FILENAME}")
foreach (TMP_PATH ${LIBRARY_SRCS})
    string (FIND ${TMP_PATH} ${EXCLUDE_FILE} EXCLUDE_FILE_FOUND)
    if (NOT ${EXCLUDE_FILE_FOUND} EQUAL -1)
        list(REMOVE_ITEM LIBRARY_SRCS ${TMP_PATH})
    endif ()
endforeach()
# set pch file names
set(PCH_FILE_HEADER "${PCH_BASE_FILENAME}.h")
set(PCH_FILE_SOURCE "${PCH_BASE_FILENAME}.cpp")

# Create visual studio groups using folder structure
foreach(source ${LIBRARY_SRCS})
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_library(${LIBRARY_NAME} STATIC
  ${LIBRARY_SRCS}
)


set(game_STAT_SRCS
  ${game_STAT_SRCS}
  ${sources_localdir}
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/dep/mersennetwister
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/zlib
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/collision
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Recast
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src/scripts/PrecompiledHeaders
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

if(UNIX)
  # Both systems don't have libdl and don't need them
  if (NOT (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD" OR CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    target_link_libraries(${LIBRARY_NAME} PUBLIC dl)
  endif()
endif()

# Define BUILD_SCRIPTDEV if need
if (BUILD_SCRIPTDEV)
  add_definitions(-DBUILD_SCRIPTDEV)
endif()

# Define BUILD_PLAYERBOT if need
if (BUILD_PLAYERBOT)
  add_definitions(-DBUILD_PLAYERBOT)
endif()
add_dependencies(game revision.h)
if (MSVC)
  set_target_properties(${LIBRARY_NAME} PROPERTIES PROJECT_LABEL "Game")
endif()

# Generate precompiled header
if(USE_COREPCH)
  target_precompile_headers(${LIBRARY_NAME} PRIVATE "${PCH_FILE_HEADER}")
endif()