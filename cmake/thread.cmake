if(THREAD STREQUAL "")
    ERROR("No thread given!")

elseif(${THREAD} STREQUAL "C++11")

elseif(${THREAD} STREQUAL "OpenMP")
    find_package(OpenMP)
    if(NOT OPENMP_FOUND)
        ERROR("OpenMP not found!")
    endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

elseif(${THREAD} STREQUAL "Serialized")

elseif(${THREAD} STREQUAL "None")

else()
    ERROR("Unknown thread given!")
endif()

INFO("Thread Support: ${THREAD}")
string(TOLOWER ${THREAD} THREAD)
string(REPLACE + p THREAD ${THREAD})
add_subdirectory(threads/${THREAD})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/threads/${THREAD})
