if(MIC)
    include(mic)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -std=c++11")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-braced-scalar-init -Wno-missing-braces")
endif()

if(PROFILING)
    set(pMR_PROFILING PROFILING)
endif()

if(WARN.ZERO)
    set(pMR_WARN_ZERO WARN.ZERO)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/config.hpp @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.hpp DESTINATION include/pmr)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include(arch)
include(thread)
include(backend)
include(provider)

# Re-configure config.hpp to set all enabled providers
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/config.hpp @ONLY)

add_subdirectory(clusters/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/clusters/common)

string(TOLOWER ${CLUSTER} CLUSTER)
add_subdirectory(clusters/${CLUSTER})

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
    $<TARGET_OBJECTS:BACKEND>
    $<TARGET_OBJECTS:CLUSTER_COMMON>
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
