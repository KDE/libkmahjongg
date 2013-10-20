project(libkmahjongg)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories(${KDE4_INCLUDES} ${CMAKE_SOURCE_DIR}/includes)

add_subdirectory( tilesets ) 
add_subdirectory( backgrounds ) 

########### next target ###############

set(kmahjongg_LIB_SRCS
   kmahjonggtileset.cpp 
   kmahjonggbackground.cpp 
   kmahjonggtilesetselector.cpp 
   kmahjonggbackgroundselector.cpp 
   kmahjonggconfigdialog.cpp )

kde4_add_ui_files(kmahjongg_LIB_SRCS kmahjonggtilesetselector.ui kmahjonggbackgroundselector.ui )

kde4_add_library(kmahjongglib SHARED ${kmahjongg_LIB_SRCS})

target_link_libraries(kmahjongglib ${KDE4_KDEUI_LIBS})

set_target_properties(kmahjongglib PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install(TARGETS kmahjongglib ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES
   libkmahjongg_export.h
   kmahjonggtileset.h
   kmahjonggbackground.h 
   kmahjonggconfigdialog.h 
DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel)

