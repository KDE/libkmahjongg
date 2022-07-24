cmake_minimum_required (VERSION 3.16 FATAL_ERROR)

project(libkmahjongg VERSION "5.1.0")

set(QT_MIN_VERSION "5.15.2")
set(KF_MIN_VERSION "5.90.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMQtDeclareLoggingCategory)

include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(FeatureSummary)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Svg
)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    ConfigWidgets
    WidgetsAddons
    Completion
    I18n
)

include(InternalMacros)

add_definitions(
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050F00
    -DQT_DEPRECATED_WARNINGS_SINCE=0x060000
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055900
    -DKF_DEPRECATED_WARNINGS_SINCE=0x060000
)

add_subdirectory(src)
add_subdirectory(tilesets)
add_subdirectory(backgrounds)

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
