cmake_minimum_required(VERSION 3.10)
project(TriangleGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Homebrew SFML 2 path
if(APPLE)
    set(SFML2_ROOT "/opt/homebrew/opt/sfml@2")
    set(SFML_INCLUDE_DIRS "${SFML2_ROOT}/include")
    set(SFML_LIBRARY_DIRS "${SFML2_ROOT}/lib")
    
    # Set pkg-config path to find SFML
    set(ENV{PKG_CONFIG_PATH} "${SFML2_ROOT}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
endif()

# Try to find SFML using pkg-config first
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(SFML QUIET sfml-graphics sfml-window sfml-system)
endif()

# If pkg-config didn't work, use manual paths
if(NOT SFML_FOUND AND APPLE)
    set(SFML_INCLUDE_DIRS "${SFML2_ROOT}/include")
    set(SFML_LIBRARY_DIRS "${SFML2_ROOT}/lib")
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
endif()

include_directories(${SFML_INCLUDE_DIRS})
link_directories(${SFML_LIBRARY_DIRS})

add_executable(TriangleGame main.cpp Game.cpp Player.cpp Obstacle.cpp Button.cpp)

target_link_libraries(TriangleGame ${SFML_LIBRARIES}) 