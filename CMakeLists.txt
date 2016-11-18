if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build disabled")
endif ()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(color)

cmake_minimum_required(VERSION 2.8.11)

project(pMR)

set(CLUSTER "" CACHE STRING "Cluster to build for.")
set(BACKEND MPI CACHE STRING "Backend to use.")
set(THREAD Serialized CACHE STRING "Threading model to use.")
set(CAPI OFF CACHE BOOL "Build C API.")
set(MIC OFF CACHE BOOL "Build for for first generation Intel Xeon Phi.")
set(PROFILING OFF CACHE BOOL "Enable profiling.")
set(ALIGNMENT 64 CACHE STRING "Set preferred memory alignment.")

#Provider specific setting (might be overwritten by cluster defaults)
set(VERBS.RDMA ON CACHE BOOL "Use RDMA capability.")
set(VERBS.MTU 1024 CACHE STRING "Set MTU size.")
set(VERBS.INLINE 128 CACHE STRING "Set max inline data size.")
set(VERBS.ODP OFF CACHE BOOL "Use ODP capability.")
set(MPI.PERSISTENT OFF CACHE BOOL "Enable persistent communication.")

set(QPACE2.WARN.TOPOLOGY ON CACHE BOOL "Warning about misuse of topology.")

add_subdirectory(src)
add_subdirectory(doxygen)
