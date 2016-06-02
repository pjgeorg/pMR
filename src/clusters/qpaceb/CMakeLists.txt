add_library(CLUSTER OBJECT
    connection.cpp
    provider.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp
    topology.cpp)

install(FILES
    connection.hpp
    provider.hpp
    recvmemorywindow.hpp
    sendmemorywindow.hpp DESTINATION include/pmr)
