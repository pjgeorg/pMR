add_library(ALLREDUCE OBJECT allreduce.cpp)

install(FILES allreduce.hpp DESTINATION include/pmr)
