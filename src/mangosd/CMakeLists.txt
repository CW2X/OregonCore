# This file is part of the MaNGOSCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB sources_localdir
  *.cpp
  RegressionTests/*.cpp
)

set(mangos-core_SRCS
  ${mangos-core_SRCS}
  ${sources_localdir}
  ${sources_Debugging}
)

if( WIN32 )
  set(mangos-core_SRCS
    ${mangos-core_SRCS}
    mangosd.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/dep/gsoap
  ${CMAKE_SOURCE_DIR}/dep/sockets/include
  ${CMAKE_SOURCE_DIR}/dep/SFMT
  ${CMAKE_SOURCE_DIR}/dep/mersennetwister
  ${CMAKE_SOURCE_DIR}/src/collision
  ${CMAKE_SOURCE_DIR}/dep/g3dlite/include
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${CMAKE_SOURCE_DIR}/src/game
  ${CMAKE_SOURCE_DIR}/dep/recastnavigation/Detour
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(mangos-core_LINK_FLAGS "")

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mangos-core ${mangos-core_SRCS})

if( NOT WIN32 )
  add_definitions(-D_MANGOS_CORE_CONFIG="${CONF_DIR}/mangoscore.conf")
endif()

add_dependencies(mangos-core revision.h)

if( UNIX )
  set(mangos-core_LINK_FLAGS "-pthread -rdynamic ${mangos-core_LINK_FLAGS}")
endif()

set_target_properties(mangos-core PROPERTIES LINK_FLAGS "${mangos-core_LINK_FLAGS}")

target_link_libraries(mangos-core
  game
  shared
  scripts
  mangosframework
  collision
  g3dlib
  gsoap
  Recast
  Detour
  ${JEMALLOC_LIBRARY}
  ${TERMCAP_LIBRARY}
  ${ACE_LIBRARY}
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${CURSES_LIBRARY}
  ${OSX_LIBS}
)

if( UNIX )
  target_link_libraries(mangos-core
    ${BFD_LIBRARY}
    dl
    ${IBERTY_LIBRARY}
    rt
  )
endif()

if( WIN32 )
  add_custom_command(TARGET mangos-core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/mangosd.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS mangos-core DESTINATION bin)
  install(FILES mangosd.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS mangos-core DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES mangosd.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
