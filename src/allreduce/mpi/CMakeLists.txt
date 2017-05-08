add_library(ALLREDUCE_MPI OBJECT allreduce.cpp)
INSTALL(FILES allreduce.hpp DESTINATION include/pmr/allreduce/mpi)
