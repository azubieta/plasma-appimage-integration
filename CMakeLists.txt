cmake_minimum_required(VERSION 3.10)
project(plasma5_appimage_extensions)

set(CMAKE_CXX_STANDARD 11)


include(FeatureSummary)

set(QT_MIN_VERSION "5.8.0")
set(KF5_MIN_VERSION "5.0.0")

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Widgets Network DBus)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS XmlGui I18n KIO TextWidgets Notifications)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMMarkNonGuiExecutable)
include(ECMOptionalAddSubdirectory)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

add_subdirectory(appimageservices-interface)
add_subdirectory(fileitem-actions)
#ecm_optional_add_subdirectory(thumbnailer)
#ecm_optional_add_subdirectory(metadata-extractor)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

