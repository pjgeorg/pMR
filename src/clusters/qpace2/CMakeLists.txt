add_library(CLUSTER OBJECT
    ../qpaceb/connection.cpp
    ../qpaceb/recvmemorywindow.cpp
    ../qpaceb/sendmemorywindow.cpp
    node.cpp
    topology.cpp)

install(FILES
    ../qpaceb/connection.hpp
    ../qpaceb/provider.hpp
    ../qpaceb/recvmemorywindow.hpp
    ../qpaceb/sendmemorywindow.hpp DESTINATION include/pmr)
