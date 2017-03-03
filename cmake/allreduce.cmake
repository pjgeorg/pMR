if(ALLREDUCE.MPI)
    set(pMR_ALLREDUCE_MPI ALLREDUCE.MPI)
    if(NOT ${BACKEND} STREQUAL "MPI")
        ERROR("MPI provider only available for backend MPI!")
    endif()

    add_subdirectory(allreduce/mpi)
    list(APPEND ALLREDUCE $<TARGET_OBJECTS:ALLREDUCE_MPI>)
    set(ALLREDUCE_STRING "${ALLREDUCE_STRING} MPI")
endif()

if(ALLREDUCE.RECURSIVE.DOUBLING)
    set(pMR_ALLREDUCE_RECURSIVE_DOUBLING ALLREDUCE.RECURSIVE.DOUBLING)
    add_subdirectory(allreduce/recursivedoubling)
    list(APPEND ALLREDUCE $<TARGET_OBJECTS:ALLREDUCE_RECURSIVE_DOUBLING>)
    set(ALLREDUCE_STRING "${ALLREDUCE_STRING} RecursiveDoubling")
endif()

INFO("Enabled AllReduce algorithm(s):${ALLREDUCE_STRING}")
