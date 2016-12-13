# pMR #
## pico Message Passing for RDMA ##
pMR is a C++ communication library particularly optimized for small messages.
It exploits communication offloading and persistence provided by the NIC, which are typical features of RDMA.
It only supports a limited low-level feature set, which allows to design a lightweight library that keeps software overhead to a minimum.

While it uses a connection-oriented API that fits quite nicely on top of many hardware-specific or hardware-near APIs, it hides all hardware specific implementation details and quirks from the user.
This approach allows the user to benefit from hardware-near optimizations while still writing hardware-agnostic code.

Apart from low-level point-to-point data transfers, pMR currently only supports global reduction.

### C API ###
In addition to the C++ API a limited C API is provided.

### Thread Support ###
pMR supports multi-threaded communication.
Thread support is limited to only one thread working on a particular connection, or a resource associated with it, simultaneously.
I.e., calls to functions associated with a particular connection have to be serialized.

### Documentation ###
pMR uses [Doxygen](http://www.doxygen.org) to generate documentation (HTML) from its annotated sources. Check out section [Compilation](#compilation) about how to build the documentation in ./doc/html.

Open current pMR API documentation: [pMR API Documentation](https://pjgeorg.github.io/pMR)

## Compilation ##
It is suggested to build pMR as a static library using the included CMake files.
Set up build environment using [CMake](http://www.cmake.org) (only out-of-source builds are allowed):

    mkdir build && cd build
    cmake \
        -DCMAKE_INSTALL_PREFIX:PATH="/install/path/for/this/library" \
        -DCLUSTER="Target cluster to build for" \
        -DBACKEND="Used backend" \
        -DTHREAD="Multithreading environment" \
        ../

### Supported options for CLUSTER ###
- <b>QPACE2</b>: Cluster with 1D FBT InfiniBand topology. InfiniBand verbs only, no shared memory.
- <b>QPACEB</b>: Cluster with single port InfiniBand HCA. InfiniBand verbs only, no shared memory.
- <b>iDataCool</b>: Cluster with single port InfiniBand HCA. InfiniBand Verbs and shared memory (Cross-Memory Attach).
- <b>SHM</b>: Single node using shared memory (Cross-Memory Attach) only.
- <b>MPI</b>: MPI fallback support. Uses MPI for intra- and inter-node communication.

### Supported options for BACKEND ###
- <b>MPI</b>: For applications using MPI for multi-processing.

### Supported options for THREAD ###
- <b>None</b>: Suitable for single-threaded applications.
- <b>Serialized</b>: Suitable for multi-threaded applications with serialized calls to pMR functions (except explicitly multi-threaded functions).
- <b>OpenMP</b>: Support for OpenMP multi-threaded applications.

### Optional configurations ###
- <b>-DMIC=ON</b>: Cross-compile for the first generation Intel Xeon Phi (KNC). Requires Intel Compiler and Intel MPSS 3.7 (or newer).
- <b>-DCAPI=ON</b>: Include optional C API.
- <b>-DPROFILING=ON</b>: Enable profiling capability. 

## Build & Install ##
To build a static library and install all required files:

    make
    make install

To build documentation (HTML):

    make doc
