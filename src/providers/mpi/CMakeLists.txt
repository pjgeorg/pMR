if("${MPI}" STREQUAL "PERSISTENT")
    message(STATUS "MPI Provider: Persistent communication enabled.")
    add_definitions(-DMPI_PERSISTENT)
endif()

add_library(PROVIDER_MPI OBJECT connection.cpp memorywindow.cpp)
