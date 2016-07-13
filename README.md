# About pMR
pMR is a C++ Library for <b>M</b>essage Passing.
It has been designed for One-sided communication, especially using <b>R</b>emote Direct Memory Access, to achieve best performance.
pMR only supports a subset of all possible communication functions, in particular only these important for LQCD software, and relies on MPI as communication and process manager.
Therefore the unit prefix <b>p</b> (pico) has been added to its name.

# Overview
Its main purpose is to hide Hardware (and Vendor) specific APIs behind a common, yet fine-grained, API with as little overhead as possible.
It uses a connection oriented communication pattern that maps quite well to various communication hardware.
This approach allows developers to have full control and push the Hardware to its limits without having to deal with any Hardware specific code.

The public API constists of two layers.
The lower-level layer implements a Window used for point to point data transfer.
The higher-level layer implements various routines that itself relies on the Window mechanism.
It is highly recommend to not use Windows for data transfer directly, but use the higher-level routines.
These higher-level routines include global reduction (Allreduce) and data exchange.

### C API ###
In addition, a limited and optional, C API is provided for C projects.

### Thread Support ###
pMR supports multithreaded communication.
Thread support is limited to only allowing one thread working on a particular connection, or a resource associated with it, simultanously.
This means calls to functions associated with a particular connection have to be serialized.

### Documentation ###
pMR uses [Doxygen](http://www.doxygen.org) to generate documentation (HTML) from its annotated sources. Check out section [Compilation](#compilation) about how to build the documentation in ./doc/html.

# Compilation
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
- <b>QPACE2</b>: Cluster with 1D Infiniband FHT topology. IB Verbs only, no shared memory.
- <b>QPACEB</b>: Cluster with single port Infiniband HCA. IB Verbs only, no shared memory.
- <b>iDataCool</b>: Cluster with single port Infiniband HCA. IB Verbs and shared memory (CMA).

### Supported options for BACKEND ###
- <b>MPI</b>: For applications using MPI for multi-processing.

### Supported options for THREAD ###
- <b>None</b>: Suitable for single-threaded applications.
- <b>Serialized</b>: Suitable for multi-threaded applications with serialized calls to pMR functions (except explicitely multi-threaded functions).
- <b>OpenMP</b>: Support for OpenMP multi-threaded applications.

### Optional configurations ###
- <b>-DMIC=ON</b>: Cross-compile for the first generation Intel Xeon Phi Architecture. Requires Intel Compiler and Intel MPSS 3.6 (or newer).
- <b>-DCAPI=ON</b>: Include optional C API.
- <b>-DPROFILING=ON</b>: Enable profiling capability. 
- <b>-DHINT=ON</b>: Enable hints that might help to spot performance issues.

## Build & Install ##
To build a static library and install all required files:

    make
    make install

To build documentation (HTML):

    make doc
