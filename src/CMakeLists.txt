add_definitions(-DTRANSLATION_DOMAIN="libkmahjongg5")

ecm_setup_version(${libkmahjongg_VERSION}
    VARIABLE_PREFIX KMAHJONGGLIB
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfigVersion.cmake"
    SOVERSION 5
)

set(kmahjongg_LIB_SRCS
   kmahjonggtileset.cpp
   kmahjonggbackground.cpp
   kmahjonggtilesetselector.cpp
   kmahjonggbackgroundselector.cpp
   kmahjonggconfigdialog.cpp
)

ecm_generate_headers(kmahjongg_LIB_CamelCase_HEADERS
    HEADER_NAMES
        KMahjonggTileset
        KMahjonggBackground
        KMahjonggConfigDialog
    REQUIRED_HEADERS kmahjongg_LIB_HEADERS
)

ecm_qt_declare_logging_category(kmahjongg_LIB_SRCS
    HEADER libkmahjongg_debug.h
    IDENTIFIER LIBKMAHJONGG_LOG
    CATEGORY_NAME log_libkmahjongg
    DESCRIPTION "libkmahjongg"
    EXPORT KMAHJONGG
)

ki18n_wrap_ui(kmahjongg_LIB_SRCS
    kmahjonggtilesetselector.ui
    kmahjonggbackgroundselector.ui
)

add_library(KF5KMahjongglib SHARED ${kmahjongg_LIB_SRCS})
generate_export_header(KF5KMahjongglib BASE_NAME libkmahjongg)

target_link_libraries(KF5KMahjongglib
    PUBLIC
        KF5::ConfigWidgets
        Qt5::Gui
    PRIVATE
        KF5::WidgetsAddons
        KF5::I18n
        KF5::Completion
        KF5::ConfigGui
        Qt5::Core
        Qt5::Svg
)

target_include_directories(KF5KMahjongglib
    INTERFACE
        "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KF5KMahjongg>"
        "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}>"
)

set_target_properties(KF5KMahjongglib PROPERTIES
    VERSION ${KMAHJONGGLIB_VERSION}
    SOVERSION ${KMAHJONGGLIB_SOVERSION}
    EXPORT_NAME KF5KMahjongglib
)


install(TARGETS KF5KMahjongglib
    EXPORT KF5KMahjonggLibraryDepends
    ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkmahjongg_export.h
    ${kmahjongg_LIB_HEADERS}
    ${kmahjongg_LIB_CamelCase_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KF5KMahjongg
    COMPONENT Devel
)

export(TARGETS KF5KMahjongglib FILE "${PROJECT_BINARY_DIR}/KF5KMahjonggLibraryDepends.cmake")

ecm_qt_install_logging_categories(
    EXPORT KMAHJONGG
    FILE libkmahjongg.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)

set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5KMahjongglib")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KMahjongglibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT KF5KMahjonggLibraryDepends
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    COMPONENT Devel
)
