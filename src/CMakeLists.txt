cmake_minimum_required(VERSION 3.0)

set(project_name RelaxMyEyes)

project(${project_name} VERSION 1.0.0.10)

set(library_name ${PROJECT_NAME}${suffix})

set(ignoreWarning ${QT_QMAKE_EXECUTABLE})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
find_package(Qt5Multimedia)

set (HEADERS
	./ApplicationHelper.h
	./LaunchAgents.h
	./MessageBoxBreak.h
	./Options.h
	./Timer.h
	./Sound.h
	./SystemIdleTime.h)

set (SOURCES
	./LaunchAgents.cpp
	./main.cpp
	./MessageBoxBreak.cpp
	./Options.cpp
	./Timer.cpp)

set (RESOURCES
	./Options.qrc
	./MessageBoxBreak.ui
	./Options.ui)

#set(MAC_LIBS)
set(EXTRA_LIBS)
set(EXTRA_INCLUDE_DIR)

IF (APPLE)
    set(SOURCES
	${SOURCES}
	./ApplicationHelper.mm
	./Sound.mm
	./SystemIdleTime.mm
    )

    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(COCOA_LIBRARY Cocoa)

    MARK_AS_ADVANCED (COCOA_LIBRARY)
    MARK_AS_ADVANCED (FOUNDATION_LIBRARY)

    set(EXTRA_LIBS
        ${FOUNDATION_LIBRARY}
        ${COCOA_LIBRARY}
    )

    find_library(SecurityFramework NAMES Security)
    if(SecurityFramework)
        list(APPEND EXTRA_LIBS ${SecurityFramework})
    endif(SecurityFramework)

    find_library(ApplicationServicesFramework NAMES ApplicationServices)
    if(ApplicationServicesFramework)
        list(APPEND EXTRA_LIBS ${ApplicationServicesFramework})
    endif(ApplicationServicesFramework)
	
    find_library(IOKIT NAMES IOKit)
    if(IOKIT)
        list(APPEND EXTRA_LIBS ${IOKIT})
    endif(IOKIT)
ENDIF (APPLE)

IF (MSVC)
    set(SOURCES
	${SOURCES}
	./ApplicationHelper.cpp
	./SystemIdleTime.cpp
    )
ENDIF (MSVC)

IF (UNIX)
    find_package(X11)
    if (NOT X11_Xscreensaver_FOUND)
        message(FATAL_ERROR "X11 Screensaver library (libXss) is required, please install libxss-dev")
    endif()

    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -no-pie -fno-exceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder -O2")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -no-pie -fno-exceptions -std=c++11 -Wno-deprecated-declarations -Wno-reorder -O2")

    set(EXTRA_LIBS
        ${EXTRA_LIBS}
        ${X11_LIBRARIES}
        ${X11_Xscreensaver_LIB}
    )

    set(EXTRA_INCLUDE_DIR
        ${EXTRA_INCLUDE_DIR}
        ${X11_INCLUDE_DIR}
    )

    set(SOURCES
        ${SOURCES}
        ./ApplicationHelper.cpp
        ./SoundUnix.cpp
        ./SystemIdleTimeUnix.cpp
    )
ENDIF (UNIX)

add_executable(${project_name} WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${RESOURCES})
set_target_properties(${project_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
target_include_directories(${project_name} SYSTEM PRIVATE ${EXTRA_INCLUDE_DIR})
target_link_libraries(${project_name} Qt5::Widgets Qt5::Multimedia ${EXTRA_LIBS})
