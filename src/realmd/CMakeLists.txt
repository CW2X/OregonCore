# This file is part of the MaNGOSCore Project. See AUTHORS file for Copyright information
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

file(GLOB sources_localdir *.cpp *.h)

set(mangos-realm_SRCS
  ${mangos-realm_SRCS}
  ${sources_localdir}
  ${sources_Debugging}
)

if( WIN32 )
  set(mangos-realm_SRCS
    ${mangos-realm_SRCS}
    realmd.rc
  )
endif()

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/src/shared
  ${CMAKE_SOURCE_DIR}/src/shared/Database
  ${CMAKE_SOURCE_DIR}/src/framework
  ${ACE_INCLUDE_DIR}
  ${MYSQL_INCLUDE_DIR}
  ${OPENSSL_INCLUDE_DIR}
)

set(mangos-realm_LINK_FLAGS "-rdynamic")

GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mangos-realm
  ${mangos-realm_SRCS}
)

add_dependencies(mangos-realm revision.h)

if( NOT WIN32 )
  add_definitions(-D_MANGOS_REALM_CONFIG="${CONF_DIR}/mangosrealm.conf")
endif()

if( UNIX )
  set(mangos-realm_LINK_FLAGS "-pthread ${mangos-realm_LINK_FLAGS}")
endif()

if( CMAKE_SYSTEM_NAME MATCHES "Darwin" )
  set(mangos-realm_LINK_FLAGS "-framework Carbon ${mangos-realm_LINK_FLAGS}")
endif()

set_target_properties(mangos-realm PROPERTIES LINK_FLAGS "${mangos-realm_LINK_FLAGS}")

target_link_libraries(mangos-realm
  shared
  mangosframework
  ${MYSQL_LIBRARY}
  ${OPENSSL_LIBRARIES}
  ${OPENSSL_EXTRA_LIBRARIES}
  ${OSX_LIBS}
)

if( UNIX )
  target_link_libraries(mangos-realm
    ${BFD_LIBRARY}
    dl
    ${IBERTY_LIBRARY}
    rt
    ${ZLIB_LIBRARIES}
  )
endif()

if( WIN32 )
  add_custom_command(TARGET mangos-realm
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/realmd.conf.dist ${CMAKE_BINARY_DIR}/bin/$(ConfigurationName)/
  )
endif()

if( UNIX )
  install(TARGETS mangos-realm DESTINATION bin)
  install(FILES realmd.conf.dist DESTINATION ${CONF_DIR})
elseif( WIN32 )
  install(TARGETS mangos-realm DESTINATION "${CMAKE_INSTALL_PREFIX}")
  install(FILES realmd.conf.dist DESTINATION "${CMAKE_INSTALL_PREFIX}")
endif()
