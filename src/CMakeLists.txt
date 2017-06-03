if(MIC)
    include(mic)
endif()

if("${CMAKE_C_FLAGS}" STREQUAL "")
    set(CMAKE_C_FLAGS "-Wall -Werror -O3")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -O3")
endif()

if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -O3 -std=c++11")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -O3 -std=c++11")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-braced-scalar-init -Wno-missing-braces")
endif()

find_package(OpenMP)
if(NOT OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    INFO("OpenMP not found. Reduce operations OpenMP SIMD optimization unavailable.")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
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
include(allreduce)

# Re-configure config.hpp to set all enabled providers
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/config.hpp @ONLY)

string(TOLOWER ${CLUSTER} CLUSTER_DIR)
add_subdirectory(clusters/${CLUSTER_DIR})

add_subdirectory(misc)

if(CAPI)
    if(NOT ${BACKEND} STREQUAL "MPI")
        ERROR("C API only available for backend MPI!")
    endif()
    add_subdirectory(api/c)
    list(APPEND API_SRC $<TARGET_OBJECTS:API_C>)
endif()

install(FILES
    allreduce.hpp
    connection.hpp
    operations.hpp
    provider.hpp
    recvmemorywindow.hpp
    recvwindow.hpp
    sendmemorywindow.hpp
    sendwindow.hpp
    window.hpp DESTINATION include/pmr)

add_library(pmr STATIC
    $<TARGET_OBJECTS:ARCH>
    $<TARGET_OBJECTS:MISC>
    $<TARGET_OBJECTS:BACKEND>
    $<TARGET_OBJECTS:CLUSTER>
    ${PROVIDERS}
    ${ALLREDUCE}
    ${API_SRC}
    allreduce.cpp
    connection.cpp
    provider.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp
    types.cpp)

target_link_libraries(pmr ${LIBRARIES})
target_include_directories(pmr PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

install(TARGETS pmr
    EXPORT pmr-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin)
install(EXPORT pmr-targets FILE pmr-config.cmake DESTINATION cmake)

add_subdirectory(tool)
