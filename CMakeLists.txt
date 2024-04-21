cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# Release Service version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "24")
set(RELEASE_SERVICE_VERSION_MINOR "04")
set(RELEASE_SERVICE_VERSION_MICRO "80")

# generate patch level from release service version
set(_micro ${RELEASE_SERVICE_VERSION_MICRO})
if (_micro LESS "10")
    string(PREPEND _micro "0") # pad with 0
endif()
set(RELEASE_SERVICE_BASED_PATCHLEVEL "${RELEASE_SERVICE_VERSION_MAJOR}${RELEASE_SERVICE_VERSION_MINOR}${_micro}")

project(libkmahjongg VERSION "6.0.${RELEASE_SERVICE_BASED_PATCHLEVEL}")

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMDeprecationSettings)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)
include(ECMGenerateExportHeader)
include(CMakePackageConfigHelpers)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Svg
)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    WidgetsAddons
    I18n
)

option(BUILD_SVG_CHECKS "Build SVG rendering checks." OFF)
add_feature_info(BUILD_SVG_CHECKS BUILD_SVG_CHECKS "Build SVG rendering checks.")

if(BUILD_SVG_CHECKS)
    find_package(ImageMagick COMPONENTS compare)
    set_package_properties(ImageMagick PROPERTIES
        TYPE REQUIRED
        PURPOSE "For checking cleaned SVG file renderings"
    )
endif()

include(InternalMacros)

ecm_set_disabled_deprecation_versions(
    QT 6.5
    KF 6.0
)

add_subdirectory(src)
add_subdirectory(tilesets)
add_subdirectory(backgrounds)

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
