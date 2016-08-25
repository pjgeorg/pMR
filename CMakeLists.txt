if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build disabled")
endif ()

cmake_minimum_required(VERSION 2.8.11)

project(pMR)

set(CLUSTER "" CACHE STRING "Cluster to build for.")
set(BACKEND "" CACHE STRING "Backend to use.")
set(THREAD "" CACHE STRING "Threading model to use.")
set(CAPI OFF CACHE BOOL "Build C API.")
set(MIC OFF CACHE BOOL "Build for Intel MIC.")
set(PROFILING OFF CACHE BOOL "Enable profiling.")
set(HINT OFF CACHE BOOL "Enable performance hints.")
set(MPI.PERSISTENT OFF CACHE BOOL "Enable persistent communication for MPI provider.")

add_subdirectory(src)
add_subdirectory(doxygen)
