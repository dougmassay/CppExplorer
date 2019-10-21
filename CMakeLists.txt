cmake_minimum_required(VERSION 3.0.0)
project(CppExplorer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS
	Core
	Widgets
	Gui
)

# find_library(libsettings REQUIRED)

add_compile_options(-std=c++11)
include_directories("${CMAKE_BINARY_DIR}/src")

add_subdirectory(src)

if (UNIX)
	install(FILES share/cpp-explorer.desktop DESTINATION /usr/share/applications)
	install(CODE "execute_process(COMMAND update-desktop-database)")
endif()
