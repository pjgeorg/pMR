add_executable(bench
    ../common/parameter.cpp
    allreduce.cpp
    exchange.cpp
    mpi.cpp
    tool.cpp
    usage.cpp)
target_link_libraries(bench pmr)
set_target_properties(bench PROPERTIES OUTPUT_NAME pmr-bench)
install(TARGETS bench DESTINATION bin)
