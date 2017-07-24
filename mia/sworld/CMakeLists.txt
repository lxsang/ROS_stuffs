project(sworld)

cmake_minimum_required(VERSION 2.8)

# Activate C++11 : 
SET(CMAKE_CXX_FLAGS "-std=c++11 -frounding-math -Wall -Wextra" )

# Boost :
find_package( Boost 1.54 COMPONENTS geometry thread graph random)
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

# CGAL :
#find_package( CGAL )
#link_directories ( ${CGAL_LIBRARY_DIRS} )
#include_directories ( ${CGAL_INCLUDE_DIRS} )

# project configuration :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../margent/src)

# Link libraries :
link_directories( ${PROJECT_SOURCE_DIR}/.. )
link_directories( ${PROJECT_SOURCE_DIR}/../margent/build )

add_library(sworld SHARED src/tools.cpp src/float2.cpp src/int2.cpp src/tools.cpp src/shape.cpp src/impact.cpp src/ogmap.cpp src/visimap.cpp)

target_link_libraries(sworld ${Boost_LIBRARIES} margent)

# install(TARGETS sworld LIBRARY DESTINATION lib)
