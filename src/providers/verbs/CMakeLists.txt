set(VERBS_SRC)
list(APPEND VERBS_SRC
    completionqueue.cpp
    connectionaddress.cpp
    connection.cpp
    context.cpp
    device.cpp
    deviceattributes.cpp
    gid.cpp
    memoryaddress.cpp
    memoryregion.cpp
    memorywindow.cpp
    portattributes.cpp
    protectiondomain.cpp
    queuepair.cpp
    topology/adapter.cpp
    topology/device.cpp
    topology/port.cpp
    topology/subnet.cpp
    topology/switch.cpp
    topology/mad/address.cpp
    topology/mad/mad.cpp
    topology/mad/queuepair.cpp
    topology/mad/switch.cpp)

#Check for ODP
include(CheckStructHasMember)
check_struct_has_member("struct ibv_device_attr_ex" odp_caps
    infiniband/verbs.h HAVE_IBVERBS_ODP LANGUAGE C)
if(HAVE_IBVERBS_ODP)
    list(APPEND VERBS_SRC memoryregion_with_odp.cpp)
else()
    list(APPEND VERBS_SRC memoryregion_without_odp.cpp)
endif()

#Check RDMA
if (VERBS.RDMA)
    add_definitions(-DVERBS_RDMA)
else()
    message(STATUS "Verbs provider: Not using RDMA.")
endif()

add_library(PROVIDER_VERBS OBJECT ${VERBS_SRC})
