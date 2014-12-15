/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

/*
 * Routine checks that the size of fundamental datatypes agree between
 * the compiler and HDF5.
 * If the HDF5 library was built with a different compiler to this, there can
 * be differences, and these would stop it working properly.
 *
 * This check is run at the start of the build process to check all is ok
 */

#include <hdf5.h>

#ifdef VERBOSE
#include <stdio.h>
#define Ereturn(str,err) { fprintf(stderr,"Type sizes for %s do no match", str); return(err); }
#else
#define Ereturn(str,err) { return(1); }
#endif

int main(int argc, char **argv)
{
  size_t csize, hsize;
  
  
  /* char */
  
  csize = sizeof(char);
  hsize = H5Tget_size(H5T_NATIVE_CHAR);
  if (csize != hsize) Ereturn ("char", 1);
  
  csize = sizeof(signed char);
  hsize = H5Tget_size(H5T_NATIVE_SCHAR);
  if (csize != hsize) Ereturn( "signed char", 2);
  
  csize = sizeof(unsigned char);
  hsize = H5Tget_size(H5T_NATIVE_UCHAR);
  if (csize != hsize) Ereturn( "unsigned char", 3);
  
  
  /* int */
  
  csize = sizeof(short int);
  hsize = H5Tget_size(H5T_NATIVE_SHORT);
  if (csize != hsize) Ereturn( "short int", 4);
  
  csize = sizeof(unsigned short int);
  hsize = H5Tget_size(H5T_NATIVE_USHORT);
  if (csize != hsize) Ereturn( "unsigned short int", 5);
  
  csize = sizeof(int);
  hsize = H5Tget_size(H5T_NATIVE_INT);
  if (csize != hsize) Ereturn( "int", 6);
  
  csize = sizeof(unsigned int);
  hsize = H5Tget_size(H5T_NATIVE_UINT);
  if (csize != hsize) Ereturn( "unsigned int", 7);
  
  csize = sizeof(long int);
  hsize = H5Tget_size(H5T_NATIVE_LONG);
  if (csize != hsize) Ereturn( "long int", 8);
  
  csize = sizeof(unsigned long int);
  hsize = H5Tget_size(H5T_NATIVE_ULONG);
  if (csize != hsize) Ereturn( "unsigned long int", 9);
  
  csize = sizeof(long long int);
  hsize = H5Tget_size(H5T_NATIVE_LLONG);
  if (csize != hsize) Ereturn( "long long int", 10);
  
  csize = sizeof(unsigned long long int);
  hsize = H5Tget_size(H5T_NATIVE_ULLONG);
  if (csize != hsize) Ereturn( "unsigned long long int", 11);
  
  
  /* float */
  
  csize = sizeof(float);
  hsize = H5Tget_size(H5T_NATIVE_FLOAT);
  if (csize != hsize) Ereturn( "float", 12);
  
  csize = sizeof(double);
  hsize = H5Tget_size(H5T_NATIVE_DOUBLE);
  if (csize != hsize) Ereturn( "double", 13);
  
  csize = sizeof(long double);
  hsize = H5Tget_size(H5T_NATIVE_LDOUBLE);
  if (csize != hsize) Ereturn( "long double", 14);
  
  
  /* HDF5 */
  
  csize = sizeof(hsize_t);
  hsize = H5Tget_size(H5T_NATIVE_HSIZE);
  if (csize != hsize) Ereturn( "hsize", 15);
  
  csize = sizeof(hssize_t);
  hsize = H5Tget_size(H5T_NATIVE_HSSIZE);
  if (csize != hsize) Ereturn( "hssize", 16);
  
  
  /* All clear */
  
#ifdef VERBOSE
  fprintf(stderr,"All ok");
#endif
  
  return (0);
}
