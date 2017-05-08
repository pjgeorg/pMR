add_library(ALLREDUCE_RECURSIVE_DOUBLING OBJECT allreduce.cpp)
INSTALL(FILES allreduce.hpp DESTINATION include/pmr/allreduce/recursivedoubling)
