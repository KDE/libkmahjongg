cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

# Release Service version, managed by release script
set(RELEASE_SERVICE_VERSION_MAJOR "25")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "2")

# generate patch level from release service version
set(_micro ${RELEASE_SERVICE_VERSION_MICRO})
if (_micro LESS "10")
    string(PREPEND _micro "0") # pad with 0
endif()
set(RELEASE_SERVICE_BASED_PATCHLEVEL "${RELEASE_SERVICE_VERSION_MAJOR}${RELEASE_SERVICE_VERSION_MINOR}${_micro}")

set(LIBKMAHJONGG_VERSION_LIB "6.0")

project(libkmahjongg VERSION "${LIBKMAHJONGG_VERSION_LIB}.${RELEASE_SERVICE_BASED_PATCHLEVEL}")
if (APPLE)
    # until now we didn't build on macos in 6.x
    # => change version for macos will not create problem
    # if we want to add new feature we will increase LIBKMAHJONGG_VERSION_LIB

    # Code stoled to kdevelop
    # The math expression below automatically increments libkmahjongg minor version in each KDE Gear release.
    # The multiplier 3 is the number of releases that share ${RELEASE_SERVICE_VERSION_MAJOR}.
    # The fraction assigns consecutive minor versions to the 3 releases - 04, 08 and 12. The rounding up groups
    # pre-release versions with the corresponding final release versions: 03 with 04, 07 with 08 and 11 with 12.
    # The final subtrahend at the end of the expression ensures version continuity: libkmahjongg 6.0 in KDE Gear 24.08.
    math(EXPR LIBKMAHJONGG_OWN_MINOR_VERSION "${RELEASE_SERVICE_VERSION_MAJOR} * 3                  \
                                          + (${RELEASE_SERVICE_VERSION_MINOR} + 4 - 3) / 4      \
                                          - 74")
    ###################################################################################
    # Kludge for bug #448152: ld: malformed 64-bit a.b.c.d.e version number: 5.7.211201
    # Apple's linker uses the project version (not the SOVERSION) to calculate an
    # unsigned int in such a way that places restrictions on the magnitude of each tier
    # of the version string.
    # See here: https://www.sicpers.info/2013/03/how-to-version-a-mach-o-library/
    ###################################################################################
    project(libkmahjongg VERSION "${LIBKMAHJONGG_VERSION_LIB}.${LIBKMAHJONGG_OWN_MINOR_VERSION}${_micro}")
endif()



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
    QT 6.9.0
    KF 6.15.0
)

add_subdirectory(src)
add_subdirectory(tilesets)
add_subdirectory(backgrounds)

ki18n_install(po)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
