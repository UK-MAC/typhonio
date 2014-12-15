/*
 Crown Copyright 2014 AWE.

 This file is part of TyphonIO. 
 Released under the BSD 3-clause license. 
 For more details see license.txt
 */
 
/*
 *  Simple unstructured mesh with 2 chunks 
 *  with Material dataset containing 6 materials + mixed cells
 *  and 2 Quant dataset
 */


#ifdef PARALLEL
#include "mpi.h"
#endif

#include "stdio.h"
#include "typhonio.h"
#include "string.h"

#define TIO_CHECK(_a, _b)\
    {\
        TIO_t        TestTIOA = (_a);\
        char     *TestMessage = (_b);\
        if (TestTIOA != TIO_SUCCESS) {\
          char text[TIO_STRLEN];\
          if (TIO_Get_Error(TestTIOA, text) != TIO_SUCCESS) {\
            strncpy(text, "*** Failed to get TyphonIO Error code ***", TIO_STRLEN);\
            text[TIO_STRLEN - 1] = '\0';\
          }\
          fprintf(stderr, "%-32s: %d (%s)\n", TestMessage, TestTIOA, text);\
        }\
        else fprintf(stderr, "%-32s: %d\n", TestMessage, TestTIOA);\
    } while (0)


int
main (int argc, char *argv[])
{
  
  const char  *fname="Test_Unstr.h5";
  const int    chunks_g = 2;


  int          i, j;
  TIO_t        terr;
  TIO_File_t   fileID;
  TIO_Object_t stateID, meshID, sectionID, matID, quantID, nquantID, vgroupID, varID;

  TIO_Size_t   nmeshes;
  TIO_Dims_t   ndims;
  TIO_Size_t   idx, nmixcell, nmixcomp, nmat;
  TIO_Size_t   nnodes, ngnodes, ncells, nshapes, nconn, ngcells, ngshapes, ngconn, nghosts;
  TIO_Xfer_t   xfer; 

  TIO_Coord_t  coordtype;
  TIO_Bool_t   ismixed;

  TIO_Data_t   datatype;
  TIO_Data_t   ncomp_datatype, ptr_datatype, vf_datatype; 
  TIO_Centre_t centring;

  TIO_Size_t   dims[TIO_MAXDIMS];
  TIO_Size_t   n1, n2, n3, n4, nchunks, ng, ichunk, nn, nobj;

  void         *mat, *ncomp, *ptr, *mixmat, *vf;
  char         matname[TIO_STRLEN], matnames[6][TIO_STRLEN], vname[TIO_STRLEN];
  int          matnums[6];

  void         *qdat, *qmix;
  char         qname[TIO_STRLEN], qnames[2][TIO_STRLEN], units[TIO_STRLEN];
  double       qmin, qmax;

  /* Material data unit and name */
  int         matarray_u[6]    = {11, 22, 33, 44, 55, 66};
  char        matnames_u[6][TIO_STRLEN] = { "Red", "Green", "Blue", "Cyan", "Magenta", "Yellow"};

  /* Chunks */

  /* Two cells(tri) + One cell (quad)+ 1 ghost */ 
  TIO_Shape_t cm3_shapes[3]   = { TIO_SHAPE_TRI3, TIO_SHAPE_QUAD4, TIO_SHAPE_QUAD4 };
  int         cm3_numcells[3] = { 2, 1, 1 };
  
  /*                                                                  G    G   */
  int         cm3_nodeids[8]  = { 1,    2,   3,     4,     5,   6,    7,   8    };
  double      cm3_x[8]        = { 0.0, 0.02, 4.01, 4.02, 8.01, 8.02, 12.01, 12.02  };
  double      cm3_y[8]        = { 1.0, 0.01, 0.11, 1.12, 0.01, 1.02,  1.11,  0.12  };

  /*                                        G*/
  int         cm3_cellids[4]  = { 1, 2, 3, 4  };
  int         cm3_conn[14]    = { 1,2,3,  1,3,4, 4,3,5,6,  5,8,7,6 };
  
  /* Chunk 2 */
  TIO_Shape_t cm4_shapes[3]   = { TIO_SHAPE_TRI3, TIO_SHAPE_QUAD4, TIO_SHAPE_QUAD4 };
  int         cm4_numcells[3] = { 2, 1, 1 };

  /*                                                                      G     G  */
  int         cm4_nodeids[8]  = { 9,     10,    7,     8,    5,     6,    3,    4   };
  double      cm4_x[8]        = { 16.01, 16.02, 12.01, 12.02, 8.01, 8.02, 4.01, 4.02 };
  double      cm4_y[8]        = { 0.01,  1.02,  1.11,  0.12,  0.01, 1.02, 0.11, 1.12 };

  /*                                       G*/
  int         cm4_cellids[4]  = { 6, 5, 4, 3 };
  int         cm4_conn[14]    = { 1,2,3, 3,4,1, 6,5,4,3, 8,7,5,6 };

  /* Total 4+4=(8), 3+3=(6), 5+4=(9), 8+8=(16), 8+8=(16), 31+24=(55) */

  /*
   *    Material with six Material types 
   *
   *     .1----.4----.6----.7----.10
   *     | \ G |     |  BC | \ Y |
   *     |RG \ |  B  |     | M \ |     
   *     .2----.3----.5----.8----.9   
   *
   *
  */

  /*                                        G*/
  int         c1_mat[4]      = {-1, 22, 33, -1};
  int         c1_ncomp[2]    = {2, 2};
  int         c1_ptr[2]      = {1, 3};
  int         c1_mixmat[4]   = {11, 22,  33, 44};
  double      c1_vf[4]       = {0.8, 0.2, 0.3, 0.7};

  /*                                         G*/
  int         c2_mat[4]      = {66, 55, -1, 33};
  int         c2_ncomp[1]    = {2}; // nmixcell
  int         c2_ptr[1]      = {1}; // 
  int         c2_mixmat[2]   = {33, 44};  // mixcomp
  double      c2_vf[2]       = {0.3, 0.7}; // 

  /* Unstr quant */
  /*                                         G*/
  double      c1_cquant[4]   = {15, 20, 30, 35};
  double      c1_cmixquant[6] = {10, 20, 20, 30, 30, 40};

  /*                                         G*/
  double      c2_cquant[4]   = {45, 40, 35, 30};
  double      c2_cmixquant[2] = {30, 40};

  /* node */
  double      c1_nquant[8]  = { 1.1,  2.2,  3.3,   4.4,  5.5,  6.6,  7.7,  8.8 };
  double      c2_nquant[8]  = { 9.9, 10.1,  7.7,   8.8,  5.5,  6.6,  3.3,  4.4 };

  /* Variable */
  double   vdata1D[2] = {1.001002, 2.002003,};
  double   ddata[2];

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


  TIO_CHECK ( terr=TIO_Create( fname, &fileID, TIO_ACC_REPLACE, "code", "vsn", "today", "title",
		     mpicomm, mpiinfo, mpirank ),   "TIO_Create");


  TIO_CHECK ( terr=TIO_Create_State( fileID, "State1", &stateID, 1, 0.1, "ms" ),
             "TIO_Create_State");


 /* Mesh 1 - unstructured Mesh with 2 chunks / ghosts */ 
    n1 = 10; n2 = 6; n3 = 4; n4 = (3*4)+(4*2);
  TIO_CHECK ( terr = TIO_Create_Mesh( fileID, stateID, "Mesh1", &meshID, TIO_MESH_UNSTRUCT, 
                         TIO_COORD_CARTESIAN, TIO_FALSE, "group1", 1, TIO_INT, 
                         TIO_DOUBLE, TIO_2D, n1, n2, n3, n4,
                         chunks_g, "iunits", "junits", "kunits", "ilabel", "jlabel", "klabel"),
                         "TIO_Create_Mesh") ;

  /* Create Vargroup with 1D variable */ 
  TIO_CHECK ( terr = TIO_Create_Vargroup(fileID, meshID, "newgroup", &vgroupID),
        "TIO_Create_Vargroup");

  dims[0] = 2;
  TIO_CHECK ( terr =  TIO_Create_Variable(fileID, vgroupID, "newvar", &varID, TIO_DOUBLE, TIO_1D, 
          dims, NULL), "TIO_Create_Variable");
    
  TIO_CHECK ( terr = TIO_Write_Variable(fileID, varID, TIO_DOUBLE, vdata1D), 
        "TIO_Write_Variable");

  TIO_CHECK ( terr = TIO_Close_Variable(fileID, varID), "TIO_Close_Variable");


  TIO_CHECK ( terr = TIO_Close_Vargroup(fileID, vgroupID), "TIO_Close_Vargroup");

  datatype = TIO_INT; 
  nghosts  = TIO_GHOSTS_ALL;
  ismixed  = TIO_TRUE;
  ncomp_datatype = TIO_INT;
  ptr_datatype   = TIO_INT;
  vf_datatype    = TIO_DOUBLE;
  nmat = 6;

  TIO_CHECK ( terr = TIO_Create_Material( fileID, meshID, "Material1", &matID, datatype, 
			nmat, nghosts, ismixed, ncomp_datatype, ptr_datatype, vf_datatype ), 
			"TIO_Create_Material") ;

  TIO_CHECK ( terr = TIO_Set_Material_Range( fileID, matID, datatype, nmat, matarray_u, matnames_u),
			"TIO_Set_Material_Range");
  
  datatype = TIO_DOUBLE;
  centring = TIO_CENTRE_CELL;
  nghosts  = TIO_GHOSTS_ALL;
  ismixed  = TIO_TRUE;
  TIO_CHECK ( terr = TIO_Create_Quant( fileID, meshID, "Quant1", &quantID, datatype, 
			centring, nghosts, ismixed, "Unit1"), "TIO_Create_Quant") ;

  centring = TIO_CENTRE_NODE; 
  ismixed  = TIO_FALSE;

  TIO_CHECK ( terr = TIO_Create_Quant( fileID, meshID, "Quant2", &nquantID, datatype, 
			centring, nghosts, ismixed, "Unit2"), "TIO_Create_Quant") ;

  qmin = 0;
  qmax = 60;
  TIO_CHECK ( terr = TIO_Set_Quant_Range(fileID, quantID, TIO_DOUBLE, &qmin, &qmax), 
			"TIO_Set_Quant_Range") ;

 
  for (ichunk=0; ichunk<chunks_g; ++ichunk){

      nnodes = 6; ncells = 3; nshapes=2; nconn=(2*3)+4; 
      ngnodes=2; ngcells=1; ngshapes=1; ngconn=4;
      if (0==ichunk) { nmixcell=2; nmixcomp= 4; }
      if (1==ichunk) { nmixcell=1; nmixcomp= 2; }

      TIO_CHECK ( terr = TIO_Set_Unstr_Chunk( fileID, meshID, ichunk, TIO_2D,
				nnodes,  ncells,  nshapes,  nconn,
				ngnodes, ngcells, ngshapes, ngconn, nmixcell, nmixcomp), 
				"TIO_Set_Unstr_Chunk");
  }
  
#ifdef XFER_COLLECTIVE
  xfer = TIO_XFER_COLLECTIVE;
#else
  xfer = TIO_XFER_INDEPENDENT;
#endif

  TIO_CHECK ( terr = TIO_Write_UnstrMesh_Chunk( fileID, meshID, 0, xfer,
                                   TIO_INT, TIO_DOUBLE, cm3_nodeids, cm3_cellids, 
                                   cm3_shapes, cm3_numcells, cm3_conn, cm3_x, cm3_y,
                                   NULL), "TIO_Write_UnstrMesh_Chunk");

  TIO_CHECK ( terr = TIO_Write_UnstrMaterial_Chunk( fileID, matID, 0, xfer,
                                       TIO_INT, c1_mat, TIO_INT, TIO_INT,
                                       TIO_DOUBLE, c1_ncomp, c1_ptr,
                                       c1_mixmat, c1_vf), "TIO_Write_UnstrMaterial_Chunk");

  TIO_CHECK ( terr = TIO_Write_UnstrQuant_Chunk( fileID, quantID, 0, xfer,
			TIO_DOUBLE, c1_cquant, c1_cmixquant),"TIO_Write_UnstrQuant_Chunk");

  TIO_CHECK ( terr = TIO_Write_UnstrQuant_Chunk( fileID, nquantID, 0, xfer,
			TIO_DOUBLE, c1_nquant, NULL),"TIO_Write_UnstrQuant_Chunk");



  TIO_CHECK ( terr = TIO_Write_UnstrMesh_Chunk( fileID, meshID, 1, xfer,
                                   TIO_INT, TIO_DOUBLE, cm4_nodeids, cm4_cellids, 
                                   cm4_shapes, cm4_numcells, cm4_conn, cm4_x, cm4_y,
                                   NULL), "TIO_Write_UnstrMesh_Chunk");


  TIO_CHECK ( terr = TIO_Write_UnstrMaterial_Chunk( fileID, matID, 1, xfer,
                                       TIO_INT, c2_mat, TIO_INT, TIO_INT,
                                       TIO_DOUBLE, c2_ncomp, c2_ptr,
                                       c2_mixmat, c2_vf), "TIO_Write_UnstrMaterial_Chunk");

  TIO_CHECK ( terr = TIO_Write_UnstrQuant_Chunk( fileID, quantID, 1, xfer,
			TIO_DOUBLE, c2_cquant, c2_cmixquant),"TIO_Write_UnstrQuant_Chunk");

  TIO_CHECK ( terr = TIO_Write_UnstrQuant_Chunk( fileID, nquantID, 1, xfer,
			TIO_DOUBLE, c2_nquant, NULL),"TIO_Write_UnstrQuant_Chunk");


  TIO_CHECK ( terr = TIO_Close_Material( fileID, matID ),  "TIO_Close_Material");

  TIO_CHECK ( terr = TIO_Close_Quant( fileID, quantID ),  "TIO_Close_Quant");

  TIO_CHECK ( terr = TIO_Close_Quant( fileID, nquantID ),  "TIO_Close_Quant");

  TIO_CHECK ( terr = TIO_Close_Mesh( fileID, meshID ),  "TIO_Close_Mesh");

  TIO_CHECK ( terr = TIO_Close_State( fileID, stateID ), "TIO_Close_State");
  
  /* End Write */
  TIO_CHECK ( terr = TIO_Close( fileID ), "TIO_Close");



  /**
   *  Read Mesh
   */
  TIO_CHECK ( terr=TIO_Open(fname, &fileID, TIO_ACC_READONLY, NULL, NULL, NULL, NULL,
                   mpicomm, mpiinfo, mpirank), "TIO_Open");

  TIO_CHECK ( terr=TIO_Open_State_idx(fileID, 0, NULL, &stateID, NULL, NULL, NULL), 
                "TIO_Open_State_idx");  

  TIO_CHECK ( terr = TIO_List_Meshes( fileID, stateID, &nmeshes, NULL), "TIO_List_Meshes");

  TIO_CHECK ( terr = TIO_Open_Mesh_idx( fileID, stateID, 0, NULL, &meshID, NULL, NULL, 
                NULL, NULL, NULL, NULL, NULL, NULL,
			    &n1, &n2, &n3, &n4,
			    NULL, NULL, NULL, NULL, NULL, NULL, NULL ), "TIO_Open_Mesh_idx");


  TIO_CHECK ( terr = TIO_Open_Vargroup_idx(fileID, meshID, 0, NULL, &vgroupID),
              "TIO_Open_Vargroup_idx");

  TIO_CHECK ( terr = TIO_Open_Variable_idx(fileID, vgroupID, 0, NULL, &varID, &datatype, 
        &ndims, NULL, NULL), "TIO_Open_Variable_idx");

  TIO_CHECK ( terr = TIO_Get_Variable_Info(fileID, varID, NULL, &datatype, 
        &ndims, dims, units), "TIO_Get_Variable_Info");

  TIO_CHECK ( terr = TIO_Read_Variable(fileID, varID, datatype, (void *)&ddata), 
        "TIO_Read_Variable");

  TIO_CHECK ( terr = TIO_Close_Variable(fileID, varID), "TIO_Close_Variable");

  TIO_CHECK ( terr = TIO_Close_Vargroup(fileID, vgroupID), "TIO_Close_Vargroup");

  /* Read Unstructured material */
  TIO_CHECK ( terr = TIO_Open_Material(fileID, meshID, "Material1", &matID, &datatype, &nmat, &nghosts,
			   &ismixed, &ncomp_datatype, &ptr_datatype, &vf_datatype), "TIO_Open_Material");


  TIO_CHECK ( terr = TIO_List_Material(fileID, meshID, matname), "TIO_List_Material");

  TIO_CHECK ( terr = TIO_Get_Material_Range( fileID, matID, datatype, &nmat, matnums, matnames),
                     "TIO_Get_Material_Range");

  /* Read material all */
  TIO_CHECK ( terr = TIO_Read_UnstrMaterial_All( fileID, matID, datatype, &ncells, NULL,
				     ncomp_datatype, ptr_datatype, vf_datatype,
				     &nmixcell, &nmixcomp,   NULL, NULL, NULL, NULL), "TIO_Read_UnstrMaterial_All");

  mat = mixmat = ncomp = ptr = vf = NULL;

  mat = (void *)malloc(ncells * TIO_SizeOf(datatype));

  if (nmixcell > 0) {
	ncomp  = (void *)malloc(nmixcell * TIO_SizeOf(ncomp_datatype));
	ptr    = (void *)malloc(nmixcell * TIO_SizeOf(ptr_datatype));
	mixmat = (void *)malloc(nmixcomp * TIO_SizeOf(datatype));
	vf     = (void *)malloc(nmixcomp * TIO_SizeOf(vf_datatype));
  }
 
  TIO_CHECK ( terr = TIO_Read_UnstrMaterial_All( fileID, matID, datatype, NULL, mat,
				     ncomp_datatype, ptr_datatype, vf_datatype,
				     NULL, NULL, ncomp, ptr, mixmat, vf ), "TIO_Read_UnstrMaterial_All");

  if (mat    != NULL) free(mat);
  if (ncomp  != NULL) free(ncomp);
  if (ptr    != NULL) free(ptr);
  if (mixmat != NULL) free(mixmat);
  if (vf     != NULL) free(vf);

  TIO_CHECK ( terr = TIO_Close_Material(fileID, matID), "TIO_Close_Material");



  /* Read Unstructured quant */
  TIO_CHECK ( terr = TIO_List_Quants( fileID, meshID, &nobj, qnames ), "TIO_List_Quants");

  for(i=0; i<nobj; ++i){

  TIO_CHECK ( terr = TIO_Open_Quant_idx(fileID, meshID, i, NULL, &quantID, 
        NULL, NULL, NULL, &ismixed, units), "TIO_Open_Quant_idx");

  TIO_CHECK ( terr = TIO_Get_Quant_Info(fileID, quantID, qname, 
        &datatype, &centring, &nghosts, &ismixed, units), "TIO_Get_Quant_Info");

  TIO_CHECK ( terr = TIO_Read_UnstrQuant_All(fileID, quantID, datatype, &nn, 
                      NULL, &nmixcomp, NULL ), "TIO_Read_UnstrQuant_All");

  qdat = qmix = NULL;

  qdat = (void *)malloc(nn * TIO_SizeOf(datatype));

  if (nmixcomp > 0) {
	qmix = (void *)malloc(nmixcomp * TIO_SizeOf(datatype));
  }

  TIO_CHECK ( terr = TIO_Read_UnstrQuant_All(fileID, quantID, datatype, NULL, 
                      qdat, NULL, qmix), "TIO_Read_UnstrQuant_All");

  if (qdat  != NULL) free(qdat);
  if (qmix  != NULL) free(qmix);
 
  TIO_CHECK ( terr = TIO_Close_Quant(fileID, quantID), "TIO_Close_Quant");

  }

#ifdef XFER_COLLECTIVE
  xfer = TIO_XFER_COLLECTIVE;
#else
  xfer = TIO_XFER_INDEPENDENT;
#endif

  /* Read material chunk */
  TIO_CHECK ( terr = TIO_Open_Material(fileID, meshID, "Material1", &matID, &datatype, &nmat, &nghosts,
			   &ismixed, &ncomp_datatype, &ptr_datatype, &vf_datatype), "TIO_Open_Material");

 TIO_CHECK ( terr = TIO_Get_nchunks(fileID, matID, &nchunks), "TIO_Get_nchunks");

    for (i=0; i<nchunks; ++i) {

      if (nghosts > 0 ) nghosts = TIO_GHOSTS_ALL;
	  else nghosts = TIO_GHOSTS_NONE;

      TIO_CHECK ( terr = TIO_Get_Unstr_Chunk( fileID, matID, i, &ndims,
			      &nnodes,  &ncells,  &nshapes,  &nconn,
			      &ngnodes, &ngcells, &ngshapes, &ngconn, &nmixcell, &nmixcomp),"TIO_Get_Unstr_Chunk") ;

      TIO_CHECK ( terr = TIO_Read_UnstrMaterial_Chunk( fileID, matID, i, xfer, datatype, nghosts,
					 &ncells, NULL, ncomp_datatype, ptr_datatype, vf_datatype,
                     &nmixcell, &nmixcomp, NULL, NULL, NULL, NULL ), "TIO_Read_UnstrMaterial_Chunk");

      mat = ncomp = ptr = mixmat = vf = NULL;

      mat = (void *)malloc((ncells + ngcells) * TIO_SizeOf(datatype));
      if (nmixcell > 0) {
	    ncomp  = (void *)malloc(nmixcell * TIO_SizeOf(ncomp_datatype));
	    ptr    = (void *)malloc(nmixcell * TIO_SizeOf(ptr_datatype));
	    mixmat = (void *)malloc(nmixcomp * TIO_SizeOf(datatype));
	    vf     = (void *)malloc(nmixcomp * TIO_SizeOf(vf_datatype));
      }

      TIO_CHECK ( terr = TIO_Read_UnstrMaterial_Chunk( fileID, matID, i, xfer, datatype, nghosts,
					 NULL, mat, ncomp_datatype, ptr_datatype, vf_datatype,
                     NULL, NULL, ncomp, ptr, mixmat, vf ), "TIO_Read_UnstrMaterial_Chunk");

      if (mat    != NULL) free(mat);
      if (ncomp  != NULL) free(ncomp);
      if (ptr    != NULL) free(ptr);
      if (mixmat != NULL) free(mixmat);
      if (vf     != NULL) free(vf);
    }

  TIO_CHECK ( terr = TIO_Close_Material(fileID, matID), "TIO_Close_Material");

  /* Read Unstructured quant chunk */
  TIO_CHECK ( terr = TIO_List_Quants( fileID, meshID, &nobj, qnames ), "TIO_List_Quants");

  for(i=0; i<nobj; ++i){

  TIO_CHECK ( terr = TIO_Open_Quant(fileID, meshID, qnames[i], &quantID, 
        NULL, NULL, NULL, &ismixed, units), "TIO_Open_Quant_idx");

  TIO_CHECK ( terr = TIO_Get_nchunks(fileID, quantID, &nchunks), "TIO_Get_nchunks");

  for (j=0; j<nchunks; ++j) {

    if (nghosts > 0 ) nghosts = TIO_GHOSTS_ALL;
	else nghosts = TIO_GHOSTS_NONE;

    TIO_CHECK ( terr = TIO_Read_UnstrQuant_Chunk(fileID, quantID, j, xfer, datatype, nghosts, 
                      &nn, NULL, &nmixcomp, NULL ), "TIO_Read_UnstrQuant_Chunk");

    qdat = qmix = NULL;

    qdat = (void *)malloc(nn * TIO_SizeOf(datatype));

    if (nmixcomp > 0) {
	  qmix = (void *)malloc(nmixcomp * TIO_SizeOf(datatype));
    }

    TIO_CHECK ( terr = TIO_Read_UnstrQuant_Chunk(fileID, quantID, j, xfer, datatype, nghosts,
                    NULL, qdat, NULL, qmix), "TIO_Read_UnstrQuant_Chunk");

    if (qdat  != NULL) free(qdat);
    if (qmix  != NULL) free(qmix);
  }

  TIO_CHECK ( terr = TIO_Close_Quant(fileID, quantID), "TIO_Close_Quant");

  }


/* CLOSE MESH */
  TIO_CHECK ( terr = TIO_Close_Mesh( fileID, meshID ), "TIO_Close_Mesh");

  TIO_CHECK ( terr = TIO_Close_State( fileID, stateID ), "TIO_Close_State");
  TIO_CHECK ( terr = TIO_Close( fileID ), "TIO_Close");

#ifdef PARALLEL
  mpierr = MPI_Finalize();
#endif
  
  return (0);
}


