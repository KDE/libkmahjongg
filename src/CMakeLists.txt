set(kmahjongg_INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KMahjongg6)
set(CMAKECONFIG_NAME "KMahjongglib6")
set(LIBRARYFILE_NAME "KMahjongg6") # no need to repeat "lib" with the actualy library file name
set(TARGET_EXPORT_NAME "KMahjongglib6")

add_definitions(-DTRANSLATION_DOMAIN="libkmahjongg6")

ecm_setup_version(${libkmahjongg_VERSION}
    VARIABLE_PREFIX KMAHJONGGLIB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/${CMAKECONFIG_NAME}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
    SOVERSION 5
)

add_library(KMahjongglib SHARED)
set_target_properties(KMahjongglib PROPERTIES
    VERSION     ${KMAHJONGGLIB_VERSION}
    SOVERSION   ${KMAHJONGGLIB_SOVERSION}
    OUTPUT_NAME ${LIBRARYFILE_NAME}
    EXPORT_NAME ${TARGET_EXPORT_NAME}
)

target_sources(KMahjongglib PRIVATE
    kmahjonggtileset.cpp kmahjonggtileset.h
    kmahjonggbackground.cpp kmahjonggbackground.h
    kmahjonggtilesetselector.cpp kmahjonggtilesetselector.h
    kmahjonggbackgroundselector.cpp kmahjonggbackgroundselector.h
    kmahjonggconfigdialog.cpp kmahjonggconfigdialog.h
)

ecm_generate_headers(kmahjongg_LIB_CamelCase_HEADERS
    HEADER_NAMES
        KMahjonggTileset
        KMahjonggBackground
        KMahjonggConfigDialog
    REQUIRED_HEADERS kmahjongg_LIB_HEADERS
)

ecm_qt_declare_logging_category(KMahjongglib
    HEADER libkmahjongg_debug.h
    IDENTIFIER LIBKMAHJONGG_LOG
    CATEGORY_NAME log_libkmahjongg
    DESCRIPTION "libkmahjongg"
    EXPORT KMAHJONGG
)

ki18n_wrap_ui(KMahjongglib
    kmahjonggtilesetselector.ui
    kmahjonggbackgroundselector.ui
)

ecm_generate_export_header(KMahjongglib
    BASE_NAME libkmahjongg
    VERSION ${libkmahjongg_VERSION}
    DEPRECATED_BASE_VERSION 0
    USE_VERSION_HEADER
    VERSION_BASE_NAME KMAHJONGGLIB
)

target_link_libraries(KMahjongglib
    PUBLIC
        KF6::ConfigWidgets
        Qt6::Gui
    PRIVATE
        KF6::WidgetsAddons
        KF6::I18n
        KF6::ConfigGui
        Qt6::Core
        Qt6::Svg
)

target_include_directories(KMahjongglib
    INTERFACE
        "$<INSTALL_INTERFACE:${kmahjongg_INCLUDE_INSTALL_DIR}>"
)


install(TARGETS KMahjongglib
    EXPORT KMahjonggTargets
    ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkmahjongg_export.h
    ${kmahjongg_LIB_HEADERS}
    ${kmahjongg_LIB_CamelCase_HEADERS}
    DESTINATION ${kmahjongg_INCLUDE_INSTALL_DIR}
    COMPONENT Devel
)

ecm_qt_install_logging_categories(
    EXPORT KMAHJONGG
    FILE libkmahjongg.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/${CMAKECONFIG_NAME}")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KMahjongglibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKECONFIG_NAME}Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKECONFIG_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${CMAKECONFIG_NAME}ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KMahjonggTargets
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    FILE ${CMAKECONFIG_NAME}Targets.cmake
    COMPONENT Devel
)
