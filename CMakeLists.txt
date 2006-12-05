project(libkmahjongg)

#add_subdirectory( carddecks ) 
#add_subdirectory( highscore ) 
#add_subdirectory( kgame ) 
#add_subdirectory( pics ) 

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/highscore/ ${CMAKE_CURRENT_SOURCE_DIR}/kgame/ ${CMAKE_CURRENT_SOURCE_DIR}/kgame/dialogs/ )


########### next target ###############

set(kmahjongg_LIB_SRCS
   kmahjonggtileset.cpp )

kde4_automoc(${kmahjongg_LIB_SRCS})

kde4_add_library(kmahjongglib SHARED ${kmahjongg_LIB_SRCS})

target_link_libraries(kmahjongglib ${KDE4_KDEUI_LIBS} kdegames ${KDE4_KNOTIFYCONFIG_LIBS} )

set_target_properties(kmahjongglib PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kmahjongglib  DESTINATION ${LIB_INSTALL_DIR} )


########### install files ###############

install(FILES
   libkmahjongg_export.h
   kmahjonggtileset.h
DESTINATION ${INCLUDE_INSTALL_DIR})

