cmake_minimum_required (VERSION 3.5 FATAL_ERROR)

project(libkmahjongg VERSION "5.0.0")

set(QT_MIN_VERSION "5.9.0")
set (KF5_MIN_VERSION "5.46.0")

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${ECM_MODULE_PATH})

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Gui
    Svg
)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    CoreAddons
    Config
    ConfigWidgets
    WidgetsAddons
    Completion
    I18n
)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(FeatureSummary)
include(GenerateExportHeader)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMQtDeclareLoggingCategory)

add_definitions(
    -DQT_NO_KEYWORDS
    -DQT_STRICT_ITERATORS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050900
    -DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x052E00
)
if (${KF5Config_VERSION} STRGREATER "5.56.0")
        add_definitions(-DQT_NO_FOREACH)
        MESSAGE(STATUS "compile without foreach")
endif()

add_subdirectory(src)
add_subdirectory(tilesets)
add_subdirectory(backgrounds)

if (${ECM_VERSION} STRGREATER "5.58.0")
    install( FILES libkmahjongg.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR} )
else()
    install( FILES libkmahjongg.categories DESTINATION ${KDE_INSTALL_CONFDIR} )
endif()

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
