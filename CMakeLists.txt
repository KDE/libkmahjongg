project(libkmahjongg)

########### next target ###############

set(kmahjongg_LIB_SRCS
   kmahjonggtileset.cpp )

kde4_automoc(${kmahjongg_LIB_SRCS})

kde4_add_library(kmahjongglib SHARED ${kmahjongg_LIB_SRCS})

target_link_libraries(kmahjongglib kdegames )

set_target_properties(kmahjongglib PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kmahjongglib  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install(FILES
   libkmahjongg_export.h
   kmahjonggtileset.h
DESTINATION ${INCLUDE_INSTALL_DIR})

