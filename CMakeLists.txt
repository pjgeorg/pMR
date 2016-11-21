if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build disabled")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(color)

cmake_minimum_required(VERSION 2.8.11)

project(pMR)

set(CLUSTER "" CACHE STRING "Cluster to build for.")
include(cluster)

set(BACKEND MPI CACHE STRING "Backend to use.")
set(THREAD Serialized CACHE STRING "Threading model to use.")
set(CAPI OFF CACHE BOOL "Build C API.")
set(MIC OFF CACHE BOOL "Build for for first generation Intel Xeon Phi.")
set(PROFILING OFF CACHE BOOL "Enable profiling.")

set(ALIGNMENT 64 CACHE STRING "Set preferred memory alignment.")
set(WARN.ZERO ON CACHE BOOL "Warn when using zero sized windows.")

set(NULL.WARN ON CACHE BOOL "Warn when using provider null.")
set(SELF.WARN ON CACHE BOOL "Warn when using provider self.")
set(VERBS.RDMA ON CACHE BOOL "Use RDMA capability.")
set(VERBS.MTU 1024 CACHE STRING "Set MTU size.")
set(VERBS.INLINE 128 CACHE STRING "Set max inline data size.")
set(VERBS.ODP OFF CACHE BOOL "Use ODP capability.")
set(OFI.EP MSG CACHE STRING "Specify endpoint type (MSG | RDM).")
set(OFI.RMA ON CACHE BOOL "Use RMA capability.")
set(OFI.RMA.EVENT OFF CACHE BOOL "Use RMA Event capability.")
set(OFI.REQUEST.MR.KEY OFF CACHE BOOL "Request unique key for memory registration.")
set(MPI.PERSISTENT OFF CACHE BOOL "Enable persistent communication.")

set(QPACE2.WARN.TOPOLOGY ON CACHE BOOL "Warning about misuse of topology.")

add_subdirectory(src)
add_subdirectory(doxygen)
