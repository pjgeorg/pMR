add_library(PROVIDER_PSM2 OBJECT
    connection.cpp
    endpoint.cpp
    error.cpp
    globalendpoint.cpp
    matchedqueue.cpp
    message.cpp
    options.cpp
    psm.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp
    stats.cpp
    units.cpp
    uuid.cpp)

if(PSM2.WRAPPER)
    add_library(pmr_psm2_preload SHARED libwrapper.c)
    install(TARGETS pmr_psm2_preload LIBRARY DESTINATION lib)
    target_link_libraries(pmr_psm2_preload dl)
endif()
