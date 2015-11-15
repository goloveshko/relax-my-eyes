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
find_package(Qt5Multimedia)

set (HEADERS
	./MessageBoxBreak.h
	./Options.h
	./Timer.h
	./Sound.h
	./SystemIdleTime.h)

set (SOURCES
	./main.cpp
	./MessageBoxBreak.cpp
	./Options.cpp
	./Timer.cpp)

set (RESOURCES
	./Options.qrc
	./MessageBoxBreak.ui
	./Options.ui)

set(MAC_LIBS)
set(EXTRA_LIBS)

IF (APPLE)
set(SOURCES
	${SOURCES}
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
		list(APPEND MAC_LIBS ${SecurityFramework})
	endif(SecurityFramework)

	find_library(ApplicationServicesFramework NAMES ApplicationServices)
	if(ApplicationServicesFramework)
		list(APPEND MAC_LIBS ${ApplicationServicesFramework})
	endif(ApplicationServicesFramework)

ENDIF (APPLE)

IF (MSVC)
set(SOURCES
	${SOURCES}
	./SystemIdleTime.cpp
)
ENDIF (MSVC)

add_executable(${project_name} WIN32 MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${RESOURCES})
set_target_properties(${project_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
target_link_libraries(${project_name} Qt5::Widgets Qt5::Multimedia ${MAC_LIBS} ${EXTRA_LIBS})
