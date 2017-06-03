if(NOT ${BACKEND} STREQUAL "MPI")
    INFO("Tool currently only available for backend MPI.")
else()
    add_subdirectory(bench)
endif()

add_subdirectory(config)
