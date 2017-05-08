function(TOSTRING OUTPUT)
    set(_TMP_RESULT "")
    set(_GLUE "")
    foreach(arg ${ARGN})
        set(_TMP_RESULT "${_TMP_RESULT}${_GLUE}${arg}")
        set(_GLUE " ")
    endforeach()
    set(${OUTPUT} "${_TMP_RESULT}" PARENT_SCOPE)
endfunction()

get_cmake_property(CACHE_VARS CACHE_VARIABLES)
foreach(CACHE_VAR ${CACHE_VARS})
    get_property(CACHE_VAR_HELPSTRING CACHE ${CACHE_VAR} PROPERTY HELPSTRING)
    if(CACHE_VAR_HELPSTRING STREQUAL "No help, variable specified on the command line.")
        get_property(CACHE_VAR_TYPE CACHE ${CACHE_VAR} PROPERTY TYPE)
        if(CACHE_VAR_TYPE STREQUAL "UNINITIALIZED")
            set(CACHE_VAR_TYPE)
        else()
            set(CACHE_VAR_TYPE :${CACHE_VAR_TYPE})
        endif()

        if("${CACHE_VAR}" STREQUAL "BACKEND" OR "${CACHE_VAR}" STREQUAL "CLUSTER" OR "${CACHE_VAR}" STREQUAL "THREAD" OR "${CACHE_VAR}" STREQUAL "CMAKE_INSTALL_PREFIX")
        else()
            LIST(FIND CMAKE_ARGS ${CACHE_VAR}${CACHE_VAR_TYPE} index)
            if(${index} GREATER -1)
                list(REMOVE_AT CMAKE_ARGS index)
                list(REMOVE_AT CMAKE_VALS index)
            endif()

            list(APPEND CMAKE_ARGS "${CACHE_VAR}${CACHE_VAR_TYPE}")
            list(APPEND CMAKE_VALS "${${CACHE_VAR}}")
        endif()
    endif()
endforeach()

set(CMAKE_ARGS ${CMAKE_ARGS} CACHE INTERNAL "CMake arguments passed by command line")
set(CMAKE_VALS ${CMAKE_VALS} CACHE INTERNAL "CMake values passed by command line")

list(LENGTH CMAKE_ARGS len)
math(EXPR len "${len} - 1")

if(len GREATER -1)
    foreach(index RANGE ${len})
        list(GET CMAKE_ARGS ${index} CMAKE_ARG)
        list(GET CMAKE_VALS ${index} CMAKE_VAL)
        list(APPEND CMAKE_CONFIG "${CMAKE_ARG}=${CMAKE_VAL}")
    endforeach()
    list(SORT CMAKE_CONFIG)
    TOSTRING(CMAKE_CONFIG ${CMAKE_CONFIG})
else()
    set(CMAKE_CONFIG "")
endif()
