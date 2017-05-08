if(CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)|(k1om)")
    add_subdirectory(arch/x86)
else()
    ERROR("Unkown architecture!")
endif()

