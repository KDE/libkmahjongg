project(libkmahjongg)

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)

set(QT_MIN_VERSION "5.7.0")
set (KF5_MIN_VERSION "5.30.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

#if(ENABLE_CLAZY)
#    set(CMAKE_CXX_COMPILER clazy)
#endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core Gui Svg)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    ConfigWidgets
    WidgetsAddons
    Completion
    I18n
)

include(FeatureSummary)
include(GenerateExportHeader)
include(ECMPackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMMarkNonGuiExecutable)
include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMQtDeclareLoggingCategory)

add_definitions(-DTRANSLATION_DOMAIN="libkmahjongg5")

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_subdirectory(tilesets)
add_subdirectory(backgrounds)

########### next target ###############

set(KMAHJONGGLIB_VERSION 5.0.0)
set(KMAHJONGGLIB_SOVERSION 5)

set(kmahjongg_LIB_SRCS
   kmahjonggtileset.cpp
   kmahjonggbackground.cpp
   kmahjonggtilesetselector.cpp
   kmahjonggbackgroundselector.cpp
   kmahjonggconfigdialog.cpp
)

ecm_qt_declare_logging_category(kmahjongg_LIB_SRCS HEADER libkmahjongg_debug.h IDENTIFIER LIBKMAHJONGG_LOG CATEGORY_NAME log_libkmahjongg)

ki18n_wrap_ui(kmahjongg_LIB_SRCS kmahjonggtilesetselector.ui kmahjonggbackgroundselector.ui )

add_library(KF5KMahjongglib SHARED ${kmahjongg_LIB_SRCS})
generate_export_header(KF5KMahjongglib BASE_NAME libkmahjongg)

target_link_libraries(KF5KMahjongglib
    PRIVATE
    Qt5::Core
    Qt5::Svg
    KF5::WidgetsAddons
    KF5::I18n
    KF5::Completion
    KF5::ConfigGui
    PUBLIC
    Qt5::Gui
    KF5::ConfigWidgets
)

target_include_directories(KF5KMahjongglib INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KF5KMahjongg>" INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}>")

set_target_properties(KF5KMahjongglib PROPERTIES VERSION ${KMAHJONGGLIB_VERSION}
                                                 SOVERSION ${KMAHJONGGLIB_SOVERSION}
                                                 EXPORT_NAME KF5KMahjongglib)

ecm_setup_version(${KMAHJONGGLIB_VERSION} VARIABLE_PREFIX KF5KMAHJONGGLIB
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfigVersion.cmake"
                        SOVERSION 6)


install(TARGETS KF5KMahjongglib EXPORT KF5KMahjonggLibraryDepends
        ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libkmahjongg_export.h
    kmahjonggtileset.h
    kmahjonggbackground.h
    kmahjonggconfigdialog.h
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KF5KMahjongg COMPONENT Devel
)

########### generate exports ###############

# add libraries to the build-tree export set
export(TARGETS KF5KMahjongglib
    FILE "${PROJECT_BINARY_DIR}/KF5KMahjonggLibraryDepends.cmake")

# define the installation directory for the CMake files
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5KMahjongglib")

# create the Config.cmake and ConfigVersion.cmake files
ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5KMahjongglibConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5KMahjongglibConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}" COMPONENT Devel
)

# install the export set for use with the install-tree
install(EXPORT KF5KMahjonggLibraryDepends
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    COMPONENT Devel
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kmahjongglib_version.h
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KF5KMahjongg
    COMPONENT Devel
)
install( FILES libkmahjongg.categories DESTINATION ${KDE_INSTALL_CONFDIR} )

########### Display Summary ###############
feature_summary(WHAT ALL
                INCLUDE_QUIET_PACKAGES
                FATAL_ON_MISSING_REQUIRED_PACKAGES)
