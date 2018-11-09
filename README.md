# TyphonIO 1.6 

## Introduction

TyphonIO is a library of routines that perform input/output (I/O) of scientific data within application codes. The library is available on HPC platforms and provides C/C++ and Fortran90 APIs to write and read TyphonIO-format files. The TyphonIO files can be dumps for restart or visualization purposes and are completely portable across HPC platforms, that is, a file written on one platform can be read on any other.

TyphonIO is based upon HDF5, a free open-source library from The HDF Group, which provides the portable data infrastructure. The way TyphonIO has been designed means that it would be possible to replace the underlying HDF5 with a successor or alternative, if the need arises, with minimal or no effect on application codes that use it.

To build TyphonIO see INSTALL for instruction.

## Using TyphonIO

To use TyphonIO, the library must be linked using ``-ltyphonio`` (and ``-ltyphonio\_f`` for Fortran)

## Documentation 

The user's manual are available in the docs/ directory, as well as the sample programs of the distribution.
