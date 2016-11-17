if(MIC)
    include(mic)
endif()

set(CMAKE_CXX_FLAGS "-Wall -O3 -std=c++11 ${CMAKE_CXX_FLAGS}")

if(PROFILING)
    add_definitions(-DPROFILING)
endif()

include(arch)
include(thread)
include(backend)
include(cluster)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/config.hpp @ONLY) 
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.hpp DESTINATION include/pmr)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(provider)

string(TOLOWER ${CLUSTER} CLUSTER)
add_subdirectory(clusters/${CLUSTER})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/clusters/${CLUSTER})

install(FILES recvwindow.hpp sendwindow.hpp window.hpp DESTINATION include/pmr)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(misc)

string(TOLOWER ${ALLREDUCE} ALLREDUCE)
add_subdirectory(allreduce/${ALLREDUCE})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/allreduce/${ALLREDUCE})

if(CAPI)
    if(NOT ${BACKEND} STREQUAL "MPI")
        ERROR("C API only available for backend MPI!")
    endif()
    add_subdirectory(api/c)
    list(APPEND API_SRC $<TARGET_OBJECTS:API_C>)
endif()

add_library(pmr STATIC
    $<TARGET_OBJECTS:ARCH>
    $<TARGET_OBJECTS:MISC>
    $<TARGET_OBJECTS:THREAD>
    $<TARGET_OBJECTS:BACKEND>
    $<TARGET_OBJECTS:CLUSTER>
    $<TARGET_OBJECTS:ALLREDUCE>
    ${PROVIDERS}
    ${API_SRC})

target_link_libraries(pmr ${LIBRARIES})
target_include_directories(pmr PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

install(TARGETS pmr
    EXPORT pmr-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)
install(EXPORT pmr-targets FILE pmr-config.cmake DESTINATION cmake)

if(NOT ${BACKEND} STREQUAL "MPI")
    INFO("Tool currently only available for backend MPI.")
else()
    add_subdirectory(tool)
endif()
