add_library(MISC OBJECT
    string.cpp
    time.cpp
    xdump.cpp)

install(FILES
    allocator.hpp
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
