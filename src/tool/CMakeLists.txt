list(REMOVE_DUPLICATES LIBRARIES)

foreach(LIB ${LIBRARIES})
    get_filename_component(LIBPATH ${LIB} PATH)
    list(APPEND LDFLAGS ${LIBPATH})
endforeach()
list(REMOVE_DUPLICATES LDFLAGS)
set(LDFLAGS_STRING -L${CMAKE_INSTALL_PREFIX}/lib)
foreach(FLAG ${LDFLAGS})
    set(LDFLAGS_STRING "${LDFLAGS_STRING} -L${FLAG}")
endforeach()

foreach(LIB ${LIBRARIES})
    get_filename_component(LIBNAME ${LIB} NAME_WE)
    list(APPEND LIBS ${LIBNAME})
endforeach()
list(REMOVE_DUPLICATES LIBS)
set(LIBS_STRING "-lpmr")
foreach(LIB ${LIBS})
    STRING(REGEX REPLACE "lib" "" LIB ${LIB})
    set(LIBS_STRING "${LIBS_STRING} -l${LIB}")
endforeach()

execute_process(COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND git log -1 --format=%H
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/info.hpp @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(tool
    benchmark.cpp info.cpp mpi.cpp parameter.cpp tool.cpp usage.cpp)
target_link_libraries(tool pmr)
set_target_properties(tool PROPERTIES OUTPUT_NAME pmr)
install(TARGETS tool DESTINATION bin)
