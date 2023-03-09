if (QT_MAJOR_VERSION STREQUAL "5")
    set(kmahjongg_INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF}/KMahjongg)
    set(CMAKECONFIG_NAME "KF5KMahjongglib")
    set(CMAKECONFIGTARGET_NAME "KF5KMahjongg") # for history reasons different from CMAKECONFIG_NAME
    set(LIBRARYFILE_NAME "KF5KMahjongglib")
    set(TARGET_EXPORT_NAME "KF5KMahjongglib")
else()
    set(kmahjongg_INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KMahjongg6)
    set(CMAKECONFIG_NAME "KMahjongglib6")
    set(CMAKECONFIGTARGET_NAME "KMahjongglib6")
    set(LIBRARYFILE_NAME "KMahjongg6") # no need to repeat "lib" with the actualy library file name
    set(TARGET_EXPORT_NAME "KMahjongglib6")
endif()

add_definitions(-DTRANSLATION_DOMAIN="libkmahjongg5")

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

if (QT_MAJOR_VERSION STREQUAL "5")
    set(_generate_export_header_version_args)
else()
    # For Qt6/KF6 world transitively include the version header
    if(ECM_VERSION VERSION_LESS "5.106")
        set(include_version_header_code "#include <kmahjongglib_version.h>\n")
        set(_generate_export_header_version_args CUSTOM_CONTENT_FROM_VARIABLE include_version_header_code)
    else()
        set(_generate_export_header_version_args USE_VERSION_HEADER VERSION_BASE_NAME KMAHJONGGLIB)
    endif()
endif()
ecm_generate_export_header(KMahjongglib
    BASE_NAME libkmahjongg
    VERSION ${libkmahjongg_VERSION}
    DEPRECATED_BASE_VERSION 0
    ${_generate_export_header_version_args}
)

target_link_libraries(KMahjongglib
    PUBLIC
        KF${KF_MAJOR_VERSION}::ConfigWidgets
        Qt${QT_MAJOR_VERSION}::Gui
    PRIVATE
        KF${KF_MAJOR_VERSION}::WidgetsAddons
        KF${KF_MAJOR_VERSION}::I18n
        KF${KF_MAJOR_VERSION}::Completion
        KF${KF_MAJOR_VERSION}::ConfigGui
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Svg
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
    FILE ${CMAKECONFIGTARGET_NAME}Targets.cmake
    COMPONENT Devel
)
