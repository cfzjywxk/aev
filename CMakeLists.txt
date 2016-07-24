cmake_minimum_required(VERSION 3.5)
project(aev, CXX)

##include dirs
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/lib)

#list include dirs
message(STATUS "******include dir in this project is******")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()
message(STATUS "******listing include dirs end******")


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")

add_subdirectory(src)
add_subdirectory(unittest)


