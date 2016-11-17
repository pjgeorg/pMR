add_library(MISC OBJECT
    ip.cpp
    string.cpp
    time.cpp
    xdump.cpp)

install(FILES
    allocator.hpp
    ip.hpp
    numeric.hpp
    print.hpp
    profiling.hpp
    random.hpp
    singleton.hpp
    string.hpp
    thread.hpp
    time.hpp
    type.hpp
    xdump.hpp DESTINATION include/pmr/misc)
