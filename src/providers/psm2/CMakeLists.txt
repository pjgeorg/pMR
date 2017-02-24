list(APPEND PSM2_COMMON
    connection.cpp
    error.cpp
    message.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp
    stats.cpp
    units.cpp)

list(APPEND PSM2_DIRECT
    direct/endpoint.cpp
    direct/globalendpoint.cpp
    direct/matchedqueue.cpp
    direct/options.cpp
    direct/psm.cpp
    direct/softendpoint.cpp
    direct/uuid.cpp)

if(PSM2.LIB STREQUAL "DIRECT")
    add_library(PROVIDER_PSM2 OBJECT ${PSM2_COMMON} ${PSM2_DIRECT})
elseif(PSM2.LIB STREQUAL "PRELOAD")
    add_library(PROVIDER_PSM2 OBJECT ${OFI_COMMON} ${PSM2_PRELOAD})
endif()
