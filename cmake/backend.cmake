if(BACKEND STREQUAL "")
    ERROR("No backend given!")

elseif(${BACKEND} STREQUAL "MPI")
    find_package(MPI)
    if(NOT MPI_C_FOUND)
        ERROR("MPI not found!")
    endif()
    #Get rid of MPICXX bindings
    add_definitions(-DMPICH_SKIP_MPICXX)
    add_definitions(-DOMPI_SKIP_MPICXX)
    include_directories(${MPI_C_INCLUDE_PATH})
    list(APPEND LIBRARIES ${MPI_C_LIBRARIES})
    add_subdirectory(backends/mpi) 
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/backends/mpi)
    INFO("Backend: MPI")

else()
    ERROR("Unknown backend given!")
endif()

