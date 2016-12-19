list(APPEND OFI_COMMON
    common/completionqueue.cpp
    common/counter.cpp
    common/domain.cpp
    common/fabric.cpp
    common/hints.cpp
    common/info.cpp
    common/memoryaddress.cpp
    common/memoryregion.cpp
    common/message.cpp
    common/rmamessage.cpp)

list(APPEND OFI_MSG
    msg/connection.cpp
    msg/endpoint.cpp
    msg/eventqueue.cpp
    msg/passiveendpoint.cpp
    msg/recvmemorywindow.cpp
    msg/sendmemorywindow.cpp)

if(OFI.EP STREQUAL "MSG")
    add_library(PROVIDER_OFI OBJECT ${OFI_COMMON} ${OFI_MSG})
elseif(OFI.EP STREQUAL "RDM")
    add_library(PROVIDER_OFI OBJECT ${OFI_COMMON} ${OFI_RDM})
endif()
