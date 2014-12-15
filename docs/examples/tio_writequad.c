/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
 
/*
 * Simple Quad mesh create, setup chunk, and write rountine
 * 
 */

#ifdef PARALLEL
#include "mpi.h"
#endif

#include "typhonio.h"


int
main (int argc, char *argv[])
{
  
  const char *fname="qfile.h5";
  
  TIO_t        terr;
  TIO_File_t   fileID;
  TIO_Object_t stateID, meshID;
  
  double xc[30] = {   1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0,  9.0, 10.0,
		     11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
		     21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0 };

  double yc[20] = {  10.0,  20.0,  30.0,  40.0,  50.0,  60.0,  70.0,  80.0,  90.0, 100.0,
		    110.0, 120.0, 130.0, 140.0, 150.0, 160.0, 170.0, 180.0, 190.0, 200.0 };

  double zc[10] = {  50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0 };
  
  float fx[3][5] = { { 1.0, 2.0, 3.0, 4.0, 5.0 },
		     { 1.3, 2.3, 3.3, 4.3, 5.3 },
		     { 1.1, 2.8, 3.1, 4.8, 5.9 } };
  
  float fy[3][5] = { { 0.1, 0.2, 0.3, 0.4, 0.5 },
		     { 1.1, 1.2, 1.3, 1.4, 1.5 },
		     { 2.1, 2.2, 2.3, 2.4, 2.5 } };
  
  
  float c1x[7][9] = { { -66.10, -66.00, -10.0, -20.0, -30.0, -40.0, -50.0, -60.0, -70.0 },
		      { -66.11, -66.01, -10.1, -20.1, -30.1, -40.1, -50.1, -60.1, -70.1 },
		      { -66.12, -66.02,   1.2,   2.2,   3.2,   4.2,   5.2,   6.2,   7.2 },
		      { -66.13, -66.03,   1.3,   2.3,   3.3,   4.3,   5.3,   6.3,   7.3 },
		      { -66.14, -66.04,   1.4,   2.4,   3.4,   4.4,   5.4,   6.4,   7.4 },
		      { -66.15, -66.05, -10.5, -20.5, -30.5, -40.5, -50.5, -60.5, -70.5 },
		      { -66.16, -66.06, -10.6, -20.6, -30.6, -40.6, -50.6, -60.6, -70.6 } };
  
  float c1y[7][9] = { { -66.10, -66.00, -10.0, -20.1, -30.2, -40.3, -50.4, -60.5, -10.6 },
		      { -66.11, -66.01, -10.0, -20.1, -30.2, -40.3, -50.4, -60.5, -70.6 },
		      { -66.12, -66.02,   0.0,   0.1,   0.2,   0.3,   0.4,   0.5,   0.6 },
		      { -66.13, -66.03,   1.0,   1.1,   1.2,   1.3,   1.4,   1.5,   1.6 },
		      { -66.14, -66.04,   2.0,   2.1,   2.2,   2.3,   2.4,   2.5,   2.6 },
		      { -66.15, -66.05, -10.0, -20.1, -30.2, -40.3, -50.4, -60.5, -70.6 },
		      { -66.16, -66.06, -10.0, -20.1, -30.2, -40.3, -50.4, -60.5, -70.6 } };
  
  float c2x[7][9] = { { -30.0, -40.0, -50.0, -60.0, -70.0, -80.0, -90.0, -99.00, -99.10 },
		      { -30.1, -40.1, -50.1, -60.1, -70.1, -80.1, -90.1, -99.01, -99.11 },
		      {   3.2,   4.2,   5.2,   6.2,   7.2,   8.2,   9.2, -99.02, -99.12 },
		      {   3.3,   4.3,   5.3,   6.3,   7.3,   8.3,   9.3, -99.03, -99.13 },
		      {   3.4,   4.4,   5.4,   6.4,   7.4,   8.4,   9.4, -99.04, -99.14 },
		      { -30.5, -40.5, -50.5, -60.5, -70.5, -80.5, -90.5, -99.05, -99.15 },
		      { -30.6, -40.6, -50.6, -60.6, -70.6, -80.6, -90.6, -99.06, -99.16 } };
  
  float c2y[7][9] = { { -30.2, -40.3, -50.5, -60.5, -70.6, -80.7, -90.8, -99.00, -99.10 },
		      { -30.2, -40.3, -50.5, -60.5, -70.6, -80.7, -90.8, -99.01, -99.11 },
		      {   0.2,   0.3,   0.4,   0.5,   0.6,   0.7,   0.8, -99.02, -99.12 },
		      {   1.2,   1.3,   1.4,   1.5,   1.6,   1.7,   1.8, -99.03, -99.13 },
		      {   2.2,   2.3,   2.4,   2.5,   2.6,   2.7,   2.8, -99.04, -99.14 },
		      { -30.2, -40.3, -50.5, -60.5, -70.6, -80.7, -90.8, -99.05, -99.15 },
		      { -30.2, -40.3, -50.5, -60.5, -70.6, -80.7, -90.8, -99.06, -99.16 } };
  
  
  
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
  
  /* Create File + State */  
  
  terr = TIO_Create( fname, &fileID, TIO_ACC_REPLACE, "code", "vsn", "today", "title",
		     mpicomm, mpiinfo, mpirank );
  
  terr = TIO_Create_State( fileID, "thestate", &stateID, 1, 0.1, "ms" );
  
  /* colinear mesh w/ 1 chunk */
  
  terr = TIO_Create_Mesh( fileID, stateID, "qmesh", &meshID, TIO_MESH_QUAD_COLINEAR,
			  TIO_COORD_CARTESIAN, TIO_FALSE, "agroup", 0,
			  TIO_DATATYPE_NULL, TIO_DOUBLE, TIO_3D,
			  30, 20, 10, TIO_NULL,
			  1,
			  "xunits", "yunits", "zunits",
			  "xlabel", "ylabel", "zlabel" );
  
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 0, TIO_3D,
			     0, 29, 0, 19, 0, 9,
			     0, 0 );
  
  /* Can only do write-all for colinear */
  terr = TIO_Write_QuadMesh_All( fileID, meshID, TIO_DOUBLE, xc, yc, zc);
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  /* colinear mesh w/ 2 chunks */
  
  terr = TIO_Create_Mesh( fileID, stateID, "qmesh2", &meshID, TIO_MESH_QUAD_COLINEAR,
			  TIO_COORD_CARTESIAN, TIO_FALSE, "agroup", 0,
			  TIO_DATATYPE_NULL, TIO_DOUBLE, TIO_3D,
			  30, 20, 10, TIO_NULL,
			  2,
			  "xunits", "yunits", "zunits",
			  "xlabel", "ylabel", "zlabel" );
  
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 0, TIO_3D,
			     0, 14, 0, 19, 0, 9,
			     0, 0 );
  
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 1, TIO_3D,
			     14, 29, 0, 19, 0, 9,
			     0, 0 );
  
  /* Can only do write-all for colinear */
  terr = TIO_Write_QuadMesh_All( fileID, meshID, TIO_DOUBLE, xc, yc, zc);
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  /* noncolinear mesh w/ 1 chunk and no ghosts */
  
  terr = TIO_Create_Mesh( fileID, stateID, "ncmesh", &meshID, TIO_MESH_QUAD_NONCOLINEAR,
			  TIO_COORD_CARTESIAN, TIO_FALSE, "agroup", 0,
			  TIO_DATATYPE_NULL, TIO_FLOAT, TIO_2D,
			  5, 3, 0, 0,
			  1,
			  "xunits", "yunits", NULL,
			  "xlabel", "ylabel", NULL );
  
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 0, TIO_2D,
			     0, 4, 0, 2, 0, 0,
			     0, 0 );
			    
  terr = TIO_Write_QuadMesh_Chunk( fileID, meshID, 0, TIO_XFER_INDEPENDENT, TIO_FLOAT, fx, fy, NULL);
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  /* another noncolinear mesh with 2 chunks and 2 layers of ghosts */
  
  terr = TIO_Create_Mesh( fileID, stateID, "cncmesh", &meshID, TIO_MESH_QUAD_NONCOLINEAR,
			  TIO_COORD_CARTESIAN, TIO_FALSE, "agroup", 0,
			  TIO_DATATYPE_NULL, TIO_FLOAT, TIO_2D,
			  9, 3, 0, 2,
			  2,
			  "xunits", "yunits", NULL,
			  "xlabel", "ylabel", NULL );
  
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 0, TIO_2D,
			     0, 4, 0, 2, 0, 0,
			     0, 0 );
			    
  terr = TIO_Set_Quad_Chunk( fileID, meshID, 1, TIO_2D,
			     4, 8, 0, 2, 0, 0,
			     0, 0 );
			    
  terr = TIO_Write_QuadMesh_Chunk( fileID, meshID, 0, TIO_XFER_INDEPENDENT, TIO_FLOAT, c1x, c1y, NULL);
  terr = TIO_Write_QuadMesh_Chunk( fileID, meshID, 1, TIO_XFER_INDEPENDENT, TIO_FLOAT, c2x, c2y, NULL);
  
  terr = TIO_Close_Mesh( fileID, meshID );
  
  
  terr = TIO_Close_State( fileID, stateID );
  
  terr = TIO_Close( fileID );
  
  
#ifdef PARALLEL
  mpierr = MPI_Finalize();
#endif
  
  return (0);
}
