cmake_minimum_required(VERSION 2.8.11)
add_executable(config
    info.cpp
    parameter.cpp
    usage.cpp)
set_target_properties(config PROPERTIES OUTPUT_NAME pmr-config)
install(TARGETS config DESTINATION bin)
