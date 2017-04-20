list(REMOVE_DUPLICATES LIBRARIES)

foreach(LIB ${LIBRARIES})
    get_filename_component(LIBPATH ${LIB} PATH)
    list(APPEND LDFLAGS ${LIBPATH})
endforeach()

list(APPEND LDFLAGS "-L${CMAKE_INSTALL_PREFIX}/lib")
list(REMOVE_DUPLICATES LDFLAGS)
TOSTRING(LDFLAGS ${LDFLAGS})

foreach(LIB ${LIBRARIES})
    get_filename_component(LIBNAME ${LIB} NAME_WE)
    STRING(REGEX REPLACE "lib" "" LIBNAME ${LIBNAME})
    list(APPEND LIBS "-l${LIBNAME}")
endforeach()
list(APPEND LIBS "-lpmr")
list(REMOVE_DUPLICATES LIBS)
TOSTRING(LIBS ${LIBS})

execute_process(COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND git log -1 --format=%H
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND git diff-index --name-only HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_MODIFIED
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(GIT_VERSION "${GIT_BRANCH} ${GIT_COMMIT_HASH}")
if(NOT GIT_MODIFIED STREQUAL "")
    set(GIT_VERSION "${GIT_VERSION} (modified)")
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/info.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(tool
    allreduce.cpp
    exchange.cpp
    info.cpp
    mpi.cpp
    parameter.cpp
    tool.cpp
    usage.cpp)
target_link_libraries(tool pmr)
set_target_properties(tool PROPERTIES OUTPUT_NAME pmr)
install(TARGETS tool DESTINATION bin)
