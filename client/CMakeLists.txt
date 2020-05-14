cmake_minimum_required(VERSION 3.5)

project(Client)
message(STATUS "Project Client")

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 1)
set (SO_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -pthread)

# Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Core)
find_package(Qt5Gui)

include_directories(${CMAKE_CURRENT_LIST_DIR}/common)

set(SOURCE_FILES
        ../common/isocketadapter.cpp
        ../common/socketadapter.cpp
        ../common/package.cpp
        ../common/logger.h
        ../common/macros.h
        ../common/common.cpp
        clientsocketadapter.cpp
        clientwidget.cpp
        main.cpp
        )

add_executable(Client ${SOURCE_FILES} ${header_files})
target_link_libraries(Client Qt5::Widgets Qt5::Network Qt5::Core Qt5::Gui )

install (TARGETS Client DESTINATION bin)
