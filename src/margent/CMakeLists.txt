project(margent)

cmake_minimum_required(VERSION 2.8)

# Activate C++11 : 
SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra" )

# project configuration :
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)

add_library(margent SHARED src/netdata.cpp src/global.cpp src/data.cpp src/agent.cpp )
target_link_libraries(margent yaml-cpp)

# add_executable(margent-test src/main.cpp src/global.cpp src/data.cpp src/agent.cpp)
# target_link_libraries(margent-test yaml-cpp)

# install(TARGETS margent RUNTIME DESTINATION bin)

# add_subdirectory(src)
