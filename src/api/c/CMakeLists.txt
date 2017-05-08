add_library(API_C OBJECT 
    connection.cpp
    recvwindow.cpp
    sendwindow.cpp)

install(FILES pmr.h DESTINATION include/pmr)
