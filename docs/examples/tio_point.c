/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
 
/*
 *  Point mesh (random particle expansion across 10 steps) with 2 materials
 */


#ifdef PARALLEL
#include "mpi.h"
#endif


#include <typhonio.h>
#include <math.h>
#include <stdio.h>

#define np_max 10000
#define nchunks 4
#define nt_steps 10

int
main (int argc, char *argv[])
{
  int it;
  TIO_Size_t nl[nchunks], nh[nchunks];
  float r = 1.0;
  int mat[np_max], nmat = 2;

#ifdef PARALLEL
  
  MPI_Comm mpicomm = MPI_COMM_WORLD;
  MPI_Info mpiinfo = MPI_INFO_NULL;
  int      mpirank;
  int      mpierr;
  
  mpierr = MPI_Init(&argc, &argv);
  mpierr = MPI_Comm_rank(mpicomm, &mpirank);
  
#else
  
  MPI_Comm mpicomm = MPI_COMM_NULL;
  MPI_Info mpiinfo = MPI_INFO_NULL;
  int      mpirank = 0;
  
#endif

  TIO_File_t fileId;
  TIO_Object_t stateId, meshId, matId, quantId;

  if (TIO_Create("pointv1.h5", &fileId, TIO_ACC_REPLACE, "pointv1",
                 "0.0", "today", "particle data", 
				 mpicomm, mpiinfo, mpirank ) != TIO_SUCCESS)
  {
    fprintf(stderr, "Failed to create file\n");
    exit(EXIT_FAILURE);
  }
  
  for (it = 0; it < nchunks; it++)
  {
    nl[it] = it * np_max / nchunks;
    nh[it] = (it + 1) * np_max / nchunks - 1;
  }
  
  for (it = 0; it < np_max; it++)
  {
    mat[it] = nmat * it / np_max + 1;
  }
  
  for (it = 0; it < nt_steps; it++)
  {
    /*
     * set up particles for each time step. Start off with point source. Particles
     * then fan out uniformaly in all directions over time steps.
     */
    char statename[10];
    float ran1, ran2, dcl, dcm, dr = 0.5;
    int ip, chunk;
    float xp[np_max], yp[np_max], val[np_max];
        
    for (ip = 0; ip < np_max; ip++)
    {
      ran1 = ((float) rand()) / RAND_MAX;
      ran2 = ((float) rand()) / RAND_MAX;

      dcl = 2.0 * ran2 - 1.0;
      dcm = sqrt(1.0 - dcl * dcl) * cos(2.0 * M_PI * ran1);

      xp[ip] = r * dcl;
      yp[ip] = r * dcm;
      val[ip] = sqrt(xp[ip] * xp[ip] + yp[ip] * yp[ip]);
    }

    r += dr;

    sprintf(statename,"State%03d", it);
    if (TIO_Create_State(fileId, statename, &stateId, it, it * 0.1, "us")
        != TIO_SUCCESS)
    {
      fprintf(stderr, "Failed to create state\n");
      exit(EXIT_FAILURE);
    }
    
    if (TIO_Create_Mesh(fileId, stateId, "Particles", &meshId, TIO_MESH_POINT,
                        TIO_COORD_CARTESIAN, TIO_FALSE, "Particles", 1, TIO_SIZE_T,
                        TIO_FLOAT, TIO_2D, np_max, 0, 0, 0, nchunks, "", "",
                        "", "x", "y", NULL) != TIO_SUCCESS)
    {
      fprintf(stderr, "Failed to create point mesh\n");
      exit(EXIT_FAILURE);
    }
    
    /*
     * Must define all chunks before writing any
     */
    for (chunk = 0; chunk < nchunks; chunk++)
    {
      if (TIO_Set_Point_Chunk(fileId, meshId, chunk, TIO_2D, nl[chunk],
                              nh[chunk], 0) != TIO_SUCCESS)
      {
        fprintf(stderr, "Failed to create point mesh chunk\n");
        exit(EXIT_FAILURE);
      }
    }
    
    if (TIO_Create_Quant(fileId, meshId, "Value", &quantId, TIO_FLOAT,
                         TIO_CENTRE_NODE, 0, TIO_FALSE, "") != TIO_SUCCESS)
    {
      fprintf(stderr, "Failed to create point quant\n");
      exit(EXIT_FAILURE);
    }
    
    if (TIO_Create_Material(fileId, meshId, "Material", &matId, TIO_INT, nmat,
                            0, TIO_FALSE, TIO_INT, TIO_INT, TIO_FLOAT)
        != TIO_SUCCESS)
    {
      fprintf(stderr, "Failed to create point material\n");
      exit(EXIT_FAILURE);
    }
    
    for (chunk = 0; chunk < nchunks; chunk++)
    {
      if (TIO_Write_PointMesh_Chunk(fileId, meshId, chunk, TIO_XFER_INDEPENDENT,
                                    TIO_FLOAT, &xp[nl[chunk]], &yp[nl[chunk]],
                                    NULL) != TIO_SUCCESS)
      {
        fprintf(stderr, "Failed to write pointmesh chunk\n");
        exit(EXIT_FAILURE);
      }
      if (TIO_Write_PointQuant_Chunk(fileId, quantId, chunk, TIO_XFER_INDEPENDENT,
                                     TIO_FLOAT, &val[nl[chunk]]) != TIO_SUCCESS)
      {
        fprintf(stderr, "Failed to write pointquant chunk\n");
        exit(EXIT_FAILURE);
      }
      if (TIO_Write_PointMaterial_Chunk(fileId, matId, chunk, TIO_XFER_INDEPENDENT,
                                        TIO_INT, &mat[nl[chunk]]) != TIO_SUCCESS)
      {
        fprintf(stderr, "Failed to write pointmaterial chunk\n");
        exit(EXIT_FAILURE);
      }
    }
    
    TIO_Close_Material(fileId, matId);
    
    TIO_Close_Quant(fileId, quantId);

    TIO_Close_Mesh(fileId, meshId);

    TIO_Close_State(fileId, stateId);  
  }

  TIO_Close(fileId);

  if (TIO_Open("pointv1.h5", &fileId, TIO_ACC_READONLY, NULL, NULL, NULL, NULL,
               mpicomm, mpiinfo, mpirank ) != TIO_SUCCESS)
  {
    fprintf(stderr, "Failed to open file\n");
    exit(EXIT_FAILURE);
  }
  if (TIO_Open_State_idx(fileId, 0, NULL, &stateId, NULL, NULL, NULL) != TIO_SUCCESS)
  {
    fprintf(stderr, "Failed to open state\n");
    exit(EXIT_FAILURE);
  }
  if (TIO_Open_Mesh_idx(fileId, stateId, 0, NULL, &meshId, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL, NULL , NULL) != TIO_SUCCESS)
  {
    fprintf(stderr, "Failed to open mesh\n");
    exit(EXIT_FAILURE);
  }
  
  if (TIO_Open_Quant(fileId, meshId, "Value", &quantId, NULL, NULL, NULL, NULL,
                     NULL) != TIO_SUCCESS)
  {
    fprintf(stderr, "Failed to open point quant\n");
    exit(EXIT_FAILURE);
  }

{
float min = -9999, max = -999;
  fprintf(stderr, "%d %f %f\n",
          TIO_Get_Quant_Range(fileId, quantId, TIO_FLOAT, &min, &max), min, max);
}

  TIO_Close_Quant(fileId, quantId);
  TIO_Close_Mesh(fileId, meshId);
  TIO_Close_State(fileId, stateId);  
  TIO_Close(fileId);

#ifdef PARALLEL
  mpierr = MPI_Finalize();
#endif
  
  return 0;
}
