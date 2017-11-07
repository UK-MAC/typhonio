# TyphonIO 1.6 (with Cmake Build)

## Introduction

TyphonIO is a library of routines that perform input/output (I/O) of scientific data within application codes. The library is available on HPC platforms and provides C/C++ and Fortran90 APIs to write and read TyphonIO-format files. The TyphonIO files can be dumps for restart or visualization purposes and are completely portable across HPC platforms, that is, a file written on one platform can be read on any other.

TyphonIO is based upon HDF5, a free open-source library from The HDF Group, which provides the portable data infrastructure. The way TyphonIO has been designed means that it would be possible to replace the underlying HDF5 with a successor or alternative, if the need arises, with minimal or no effect on application codes that use it.

## Pre-requisites

To build TyphonIO you will require: 

```
MPI
CMake Version 2.8.9 or higher
Hierachical Data Format (HDF5) 
Optionally:
    Fortran Compiler (to build Fortran library
    Doxygen (to generate doxygen based documenation)
```

## Building

TyphonIO can be build with the following steps:

```
mkdir build
cd build
cmake [optional cmake flags] ..
make install
```

The build configuration can be changed by passing certain flags to cmake, these include:
```
-DCMAKE_INSTALL_PREFIX=<Install Location>
-DBUILD_TIO_SHARED=[ON|OFF] - Default OFF
-DBUILD_FORTRAN_LIBRARY=[ON|OFF] - Default OFF
-DBUILD_DOXYGEN_DOCS=[ON|OFF] - Default OFF
-DBUILD_USER_GUIDE=[ON|OFF] - Deafult ON
```

## Using TyphonIO

To use TyphonIO, the library must be linked using ``-ltyphonio`` (and ``-ltyphonio_f`` for Fortran)


