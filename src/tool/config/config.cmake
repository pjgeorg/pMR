cmake_minimum_required(VERSION 2.8.11)

if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -O3 -std=c++11")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -std=c++11")
endif()

add_executable(config
    info.cpp
    parameter.cpp
    usage.cpp)
set_target_properties(config PROPERTIES OUTPUT_NAME pmr-config)
install(TARGETS config DESTINATION bin)
