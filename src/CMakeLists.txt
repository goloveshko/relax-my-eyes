cmake_minimum_required(VERSION 3.0)

set(project_name RelaxMyEyes)

project(${project_name} VERSION 1.0.0.10)

set(library_name ${PROJECT_NAME}${suffix})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)

add_executable(${project_name} WIN32 MACOSX_BUNDLE main.cpp MessageBoxBreak.cpp MessageBoxBreak.h MessageBoxBreak.ui Options.cpp Options.h Options.ui Timer.cpp Timer.h Options.qrc)
set_target_properties(${project_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
target_link_libraries(${project_name} Qt5::Widgets)