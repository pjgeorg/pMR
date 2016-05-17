include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(process_vm_writev HAVE_CMA)
if(NOT HAVE_CMA)
    message(FATAL_ERROR "Cross Memory Attach not available!")
endif()

add_library(PROVIDER_CMA OBJECT connection.cpp memorywindow.cpp)
