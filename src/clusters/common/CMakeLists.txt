add_library(CLUSTER_COMMON OBJECT
    connection.cpp
    provider.cpp
    recvmemorywindow.cpp
    sendmemorywindow.cpp
    types.cpp)

install(FILES
    connection.hpp
    provider.hpp
    recvmemorywindow.hpp
    sendmemorywindow.hpp DESTINATION include/pmr)
