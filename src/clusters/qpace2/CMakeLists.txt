add_library(CLUSTER OBJECT
    connect.cpp
    node.cpp
    topology.cpp)

if(QPACE2.WARN.TOPOLOGY)
    add_definitions(-DQPACE2_WARN_TOPOLOGY)
endif()
