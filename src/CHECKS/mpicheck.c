/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */

#include <mpi.h>

int main(int argc, char **argv)
{
  MPI_Comm comm;
  int mpierr, result;

  mpierr = MPI_Init(&argc, &argv);
  mpierr = MPI_Comm_dup(MPI_COMM_WORLD, &comm);
  mpierr = MPI_Comm_compare(comm, MPI_COMM_NULL, &result);
  mpierr = MPI_Comm_compare(MPI_COMM_NULL, comm, &result);
  mpierr = MPI_Comm_free(&comm);
  mpierr = MPI_Finalize();

  return (0);
}

