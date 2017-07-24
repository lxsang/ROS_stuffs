project(test)
set(VERSION_STRING "0.4")

cmake_minimum_required(VERSION 2.8)

# Activate C++11 : 
SET(CMAKE_CXX_FLAGS "-std=c++11 -frounding-math -Wall -Wextra" )

# Boost :
find_package( Boost 1.54 COMPONENTS thread graph)
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

# Get SDL2 :
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})

# Link libraries :
link_libraries( ${Boost_LIBRARIES} )
link_directories( ${PROJECT_SOURCE_DIR}/../sworld/build )
link_directories( ${PROJECT_SOURCE_DIR}/../margent/build )

# project configuration :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../sworld/src)
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../margent/src)

# En mode debug :
add_executable(test src/control.cpp src/frame.cpp src/main.cpp)
target_link_libraries(test ${Boost_LIBRARIES} ${SDL2_LIBRARIES} SDL2_gfx margent sworld)
