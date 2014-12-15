/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
 
/*
 * Simple Quad mesh Open, Get chunk, and read rountine
 * 
 */

#ifdef PARALLEL
#include "mpi.h"
#endif

#include "typhonio.h"

static TIO_Size_t
product(const TIO_Size_t n1, const TIO_Size_t n2, const TIO_Size_t n3)
{
  TIO_Size_t n;
  
  n = 1;
  
  if (n3 > 0) n *= n3;
  if (n2 > 0) n *= n2;
  n *= n1;
  
  return (n);
}


int
main (int argc, char *argv[])
{
  
  const char *fname="qfile.h5";
  
  TIO_t        terr;
  TIO_File_t   fileID;
  TIO_Object_t stateID, meshID;
  TIO_Size_t   ni, nj, nk, n, nchunks, ichunk, nghosts, ng;
  TIO_Size_t   il, ih, jl, jh, kl, kh;
  TIO_Dims_t   ndims;
  float *rx, *ry, *rz;

  TIO_Size_t   nmeshes, imesh;
  TIO_Mesh_t   meshtype;
  char         meshname[TIO_STRLEN];
  
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
  
  /* Open file, Read only option*/
  
  terr = TIO_Open( fname, &fileID, TIO_ACC_READONLY, NULL, NULL, NULL, NULL,
		   mpicomm, mpiinfo, mpirank);
  
  /* Open first state of the file */
  terr = TIO_Open_State_idx( fileID, 0, NULL, &stateID, NULL, NULL, NULL );
  
  /* 3D colinear quadmesh - single chunk - add some ghosts */
  
  terr = TIO_Open_Mesh( fileID, stateID, "qmesh", &meshID, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL,
			&ni, &nj, &nk, NULL,
			&nchunks,
			NULL, NULL, NULL, NULL, NULL, NULL );
  
  for (ng=0; ng<=2; ++ng) {
    for (ichunk=0; ichunk<nchunks; ++ichunk) {
      
      terr = TIO_Get_Quad_Chunk( fileID, meshID, ichunk, &ndims, &il, &ih, &jl, &jh, &kl, &kh, NULL, NULL);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, &nk, NULL, NULL, NULL);
      
      rx = (float *)malloc(sizeof(float) * ni);
      ry = (float *)malloc(sizeof(float) * nj);
      rz = (float *)malloc(sizeof(float) * nk);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, &nk, rx, ry, rz);
      
      free (rx);
      free (ry);
      free (rz);
    }
  }
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  
  
  /* 3D colinear quadmesh - two chunks - add some ghosts */
  
  terr = TIO_Open_Mesh( fileID, stateID, "qmesh2", &meshID, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL,
			&ni, &nj, &nk, NULL,
			&nchunks,
			NULL, NULL, NULL, NULL, NULL, NULL );
  
  for (ng=0; ng<=2; ++ng) {
    for (ichunk=0; ichunk<nchunks; ++ichunk) {
      
      terr = TIO_Get_Quad_Chunk( fileID, meshID, ichunk, &ndims, &il, &ih, &jl, &jh, &kl, &kh, NULL, NULL);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, &nk, NULL, NULL, NULL);
      
      rx = (float *)malloc(sizeof(float) * ni);
      ry = (float *)malloc(sizeof(float) * nj);
      rz = (float *)malloc(sizeof(float) * nk);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, &nk, rx, ry, rz);
      
      free (rx);
      free (ry);
      free (rz);
    }
  }
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  
  
  /* 2D non-colinear quadmesh - single chunk, stored with no ghosts */
  
  terr = TIO_Open_Mesh( fileID, stateID, "ncmesh", &meshID, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL,
			&ni, &nj, NULL, &nghosts,
			&nchunks,
			NULL, NULL, NULL, NULL, NULL, NULL );
  
  for (ichunk=0; ichunk<nchunks; ++ichunk) {
    
    terr = TIO_Get_Quad_Chunk( fileID, meshID, ichunk, &ndims, &il, &ih, &jl, &jh, &kl, &kh, NULL, NULL);
    
    terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, nghosts, NULL, &ni, &nj, NULL, NULL, NULL, NULL);
    
    n = ni * nj;
    
    rx = (float *)malloc(sizeof(float) * n);
    ry = (float *)malloc(sizeof(float) * n);
    
    terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, nghosts, NULL, &ni, &nj, NULL, rx, ry, NULL);
    
    free (rx);
    free (ry);
    
  }
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  
  
  /* 2D non-colinear quadmesh - two chunks, stored with 2 ghosts */
  
  
  terr = TIO_Open_Mesh( fileID, stateID, "cncmesh", &meshID, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, NULL,
			&ni, &nj, NULL, &nghosts,
			&nchunks,
			NULL, NULL, NULL, NULL, NULL, NULL );
  
  for (ng=0; ng<=nghosts; ++ng) {
    for (ichunk=0; ichunk<nchunks; ++ichunk) {
      
      terr = TIO_Get_Quad_Chunk( fileID, meshID, ichunk, &ndims, &il, &ih, &jl, &jh, &kl, &kh, NULL, NULL);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, NULL, NULL, NULL, NULL);
      
      n = ni * nj;
      
      rx = (float *)malloc(sizeof(float) * n);
      ry = (float *)malloc(sizeof(float) * n);
      
      terr = TIO_Read_QuadMesh_Chunk( fileID, meshID, ichunk, TIO_XFER_NULL, TIO_FLOAT, ng, NULL, &ni, &nj, NULL, rx, ry, NULL);
      
      free (rx);
      free (ry);
    }
  }
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  /* finish */
  
  terr = TIO_Close_State( fileID, stateID );
  terr = TIO_Close( fileID );
  

  /* Reopen the file */
  terr = TIO_Open( fname, &fileID, TIO_ACC_READONLY, NULL, NULL, NULL, NULL,
		   mpicomm, mpiinfo, mpirank);
  
  terr = TIO_Open_State_idx( fileID, 0, NULL, &stateID, NULL, NULL, NULL );
  
  /* List number of Meshes in state */
  terr = TIO_List_Meshes( fileID, stateID, &nmeshes, NULL);

  /* Loop through all meshes */
  for (imesh=0; imesh<nmeshes; ++imesh) {
    
    terr = TIO_Open_Mesh_idx( fileID, stateID, imesh, meshname, &meshID, &meshtype, NULL,
			      NULL, NULL, NULL, NULL, NULL,
			      &ndims, &ni, &nj, &nk, NULL, NULL,
			      NULL, NULL, NULL, NULL, NULL, NULL );
    
    switch (meshtype) {
    case TIO_MESH_QUAD_COLINEAR:
      break;
    case TIO_MESH_QUAD_NONCOLINEAR:
      n = product(ni, nj, nk);
      ni = nj = nk = n;
      break;
    }
    
    rx = ry = rz = NULL;
    switch (ndims) {
    case TIO_3D:
      rz = (float *)malloc(sizeof(float) * nk);
    case TIO_2D:
      ry = (float *)malloc(sizeof(float) * nj);
    case TIO_1D:
      rx = (float *)malloc(sizeof(float) * ni);
    }
    
    /* Read all mesh */
    terr = TIO_Read_QuadMesh_All( fileID, meshID, TIO_FLOAT, NULL, &ni, &nj, &nk, rx, ry, rz);
    
    
    if (rz != NULL) free (rz);
    if (ry != NULL) free (ry);
    if (rx != NULL) free (rx);
    
    terr = TIO_Close_Mesh( fileID, meshID );
  }
  
  
  terr = TIO_Close_State( fileID, stateID );
  terr = TIO_Close( fileID );



#ifdef PARALLEL
  mpierr = MPI_Finalize();
#endif
  
  return (0);
}
