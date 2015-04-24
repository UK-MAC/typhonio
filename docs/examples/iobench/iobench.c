/*----------------------------------------------------------------------
 *  IOBENCH - Version 1.5
 *
 * Crown Copyright 2015 AWE.
 *
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *   * Neither the name of AWE Plc nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without specific
 *     prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, NCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR  CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 *----------------------------------------------------------------------

    Benchmark code to test IO performance using the TyphonIO Version 1.X library

    Models Quant, Point and Unstructured mesh I/O

    Usage:   iobench.x <type> <nr> <nm> <ng> <nq> <nv> <nn> <mode> <reps> 
                       <c|s|n|ci> <np> [filename]

     Arg#1       <type>    = type of mesh (q = quad, u = unstructured, p = point)
     Arg#2       <nr>      = No. real material and quant elements
                             per process and is a multiple of 100
     Arg#3       <nm>      = No. mixed material and quant elements per process and is a
                             multiple of 100 (each mixed cell will have 2 components)
     Arg#4       <ng>      = No. ghost cells per process and is a multiple of 100.
                             For Quad meshes = No. ghost layers
     Arg#5       <nq>      = No. Quantities to be written
     Arg#6       <nv>      = No. Variables to be written. Will write 1 0D
                             and 1 1D var (size n) of type int and double
                             (4 total) per requested variable.
     Arg#7       <mode>    = I/O mode. One of:
                 r         = specifies read
                 w         = specifies write
                 a         = specifies append
                 as        = specifies append to state (separate files). Requires nv>0
                 aas       = specifies append to file and state. Requires nv>0
                 ra        = specifies read appended
                 pw        = specifies plfs write
                 pr        = specifies plfs read
                 fpcw      = specifies file per core write
                 fpcr      = specifies file per core read
     Arg#8       reps      = specifies the number of repetitions
     Arg#9       <c|s|n|ci>= read chunks (c) only, sections (s) only or neither (n).
                             For writing point and unstructured, (ci) specifies that 
                             chunks will be incremented by steps of 100.
                             (e.g. if nr=100, proc=0 has 100, proc=1 has 200, proc=3 has 300 etc)
     Arg#10      <np>      = Number of procs writing/reading.
                             Note: subset writing cannot be used for fpcw/r.
     Arg#11    [filename]  = optional - user defined filename.
                             can be used for testing section reads on
                             different numbers of processors (if s set above).
                             or specifying the mount point for PLFS filenames
                             (for pr or pw options above)
      e.g. mpirun -np 8 iobench.x u 1000 0 0 5 0 0 0 w 10 c 2
           Runs on 8PE but only writes on 2.
           Each processor writes 1000 cells and 5 Quants
           The program repeats 10 times, writing 10 files
           Rerunning with the mode set to 'r' will read the 10 files
           Running with the mode set to 'a' writes 1 file with 10 dumps

-------------------------------------------------------------------------------------------------*/

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "time.h"

#ifdef PARALLEL
#include "mpi.h"
#endif

#include "typhonio.h"

void chkint(char *, int *, int *, int);
void chkdbl(char *, double *, double *, int);
int main_single(int, char *[], int);

char  errstr[TIO_STRLEN];
TIO_t errnum;

#define TIO_Call(r,s) if((errnum=r) != TIO_SUCCESS) {TIO_Get_Error(errnum, errstr); printf("%s\n%s\n",s,errstr); exit(EXIT_FAILURE);}
#define tmalloc(s,t,u) if ((s = malloc(t)) == NULL) {fprintf(stderr, u); exit(EXIT_FAILURE);}

char version[TIO_STRLEN] = "1.5.3";

enum { UNSET, READ, WRITE, APPEND, READAPP, PLFS_READ,  PLFS_WRITE, 
       FPC_WRITE, FPC_READ, APP_ST, APP_APP_ST } mode;
enum { NONE, QUAD, UNSTRUCT, POINT} meshtype;
enum { NEITHER, CHUNK, SECTION } rflag;
enum { SAME, INCREMENT } sflag;
enum { AUTO, USER} fflag;

static int   me, np;
int          mpirank, mpisize, mpisizeall, mpierr;

/*TIO_Xfer_t TIO_XFER = TIO_XFER_INDEPENDENT; */
TIO_Xfer_t TIO_XFER = TIO_XFER_COLLECTIVE;

#ifdef PARALLEL

MPI_Comm     mpicomm;
MPI_Comm     mpicomm_grp;
MPI_Info     mpiinfo;

#else

int    idummy() { return(0); }
double ddummy() { return(0.0); }

int          mpicomm, mpicomm_grp, mpiinfo;

#define MPI_COMM_WORLD -1
#define MPI_DOUBLE     -1
#define MPI_MAX        -1

#define MPI_Init(a,b)             idummy()
#define MPI_Finalize()            idummy()
#define MPI_Comm_rank(a,b)        idummy()
#define MPI_Comm_size(a,b)        idummy()
#define MPI_Barrier(a)            idummy()
#define MPI_Info_create(a)        idummy()
#define MPI_Info_set(a,b,c)       idummy()
#define MPI_Info_free(a)          idummy()
#define MPI_Reduce(a,b,c,d,e,f,g) idummy()

#define MPI_Wtime()               ddummy();

#endif

void
usage (int rank)
{

  if (rank == 0) {
    fprintf(stderr, "\nUsage:   iobench.x <type> <nr> <nm> <ng> <nq> <nv> <ns> <nn> <mode> <reps> <c|s|n> <np> [filename]\n");
    fprintf(stderr, "   Arg#1       <type>    = type of mesh (q = quad, u = unstructured, p = point)\n");
    fprintf(stderr, "   Arg#2       <nr>      = No. real material and quant elements\n");
    fprintf(stderr, "                           per process and is a multiple of 100\n");
    fprintf(stderr, "   Arg#3       <nm>      = No. mixed material and quant elements per process and is a\n");
    fprintf(stderr, "                           multiple of 100 (each mixed cell will have 2 components)\n");
    fprintf(stderr, "   Arg#4       <ng>      = No. ghost cells per process and is a multiple of 100.\n");
    fprintf(stderr, "                           For Quad meshes = No. ghost layers\n");
    fprintf(stderr, "   Arg#5       <nq>      = No. Quantities to be written\n");
    fprintf(stderr, "   Arg#6       <nv>      = No. Variables to be written. Will write 1 0D\n");
    fprintf(stderr, "                           and 1 1D var (size n) of type int and double\n");
    fprintf(stderr, "                           (4 total) per requested variable.\n");
    fprintf(stderr, "   Arg#7       <mode>    = I/O mode. One of:\n");
    fprintf(stderr, "               r         = specifies read\n");
    fprintf(stderr, "               w         = specifies write\n");
    fprintf(stderr, "               a         = specifies append\n");
    fprintf(stderr, "               as        = specifies append to state (separate files)\n");
    fprintf(stderr, "               aas       = specifies append to file and state\n");
    fprintf(stderr, "               ra        = specifies read appended\n");
    fprintf(stderr, "               pw        = specifies plfs write\n");
    fprintf(stderr, "               pr        = specifies plfs read\n");
    fprintf(stderr, "               fpcw      = specifies file per core write\n");
    fprintf(stderr, "               fpcr      = specifies file per core read\n");
    fprintf(stderr, "   Arg#8       reps      = specifies the number of repetitions \n");
    fprintf(stderr, "   Arg#9       <c|s|n|ci>= read chunks (c) only, sections (s) only or neither (n).\n");
    fprintf(stderr, "                           For writing point and unstructured, (ci) specifies that \n");
    fprintf(stderr, "                           chunks will be incremented by steps of 100.\n");
    fprintf(stderr, "                           (e.g. if nr=100, proc=0 has 100, proc=1 has 200, proc=3 has 300 etc)\n");
    fprintf(stderr, "   Arg#10      <np>      = Number of procs writing/reading.\n");
    fprintf(stderr, "                           Note: subset writing cannot be used for fpcw/r.\n");
    fprintf(stderr, "   Arg#11    [filename]  = optional - user defined filename.\n");
    fprintf(stderr, "                           can be used for testing section reads on\n");
    fprintf(stderr, "                           different numbers of processors (if s set above).\n");
    fprintf(stderr, "                           or specifying the mount point for PLFS filenames\n");
    fprintf(stderr, "                           (for pr or pw options above)\n\n");
    fprintf(stderr, "e.g. mpirun.awe -np 8 iobench.x u 1000 0 0 5 0 0 0 w 10 c 2\n\n");
    fprintf(stderr, "     Runs on 8PE but only writes on 2.\n");
    fprintf(stderr, "     Each processor writes 1000 cells and 5 Quants\n");
    fprintf(stderr, "     The program repeats 10 times, writing 10 files\n\n");
    fprintf(stderr, "     Rerunning with the mode set to 'r' will read the 10 files\n");
    fprintf(stderr, "     Running with the mode set to 'a' writes 1 file with 10 dumps\n");
  }

  mpierr = MPI_Barrier(MPI_COMM_WORLD);
  exit(EXIT_FAILURE);

  return;
}


int main(int argc, char *argv[])
{
  int outnum;
  int maxnum;
  int *ranks;
  MPI_Group mpinewgrp, mpioldgrp;
  int i;

  mpierr  = MPI_Init(&argc, &argv);	
  mpierr  = MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
  mpierr  = MPI_Comm_size(MPI_COMM_WORLD, &mpisizeall);
  if (argc > 10) {
    np = atoi(argv[10]);
  }
  else{
    np = mpisizeall;
  }
  if ((strcasecmp("fpcw", argv[7]) == 0) || (strcasecmp("fpcr", argv[7]) == 0)) {
    mpicomm     = MPI_COMM_SELF;
    mpicomm_grp = MPI_COMM_SELF;
    me          = 0;
    mpisize     = 1;
  } else if (np != mpisizeall) {
    if (np > 0 && np < mpisizeall) {
      mpicomm     = MPI_COMM_WORLD;
      mpierr      = MPI_Comm_group(mpicomm, &mpioldgrp);
      tmalloc(ranks, (sizeof(int) * np), "Failed to allocate intvar" );
      for (i=0; i < np; ++i) {
        ranks[i] = i;
      }
      mpierr      = MPI_Group_incl(mpioldgrp, np, ranks, &mpinewgrp);
      free(ranks);
      mpierr      = MPI_Comm_create(mpicomm, mpinewgrp, &mpicomm_grp);
      if (mpirank < np) {
        mpierr      = MPI_Comm_rank(mpicomm_grp, &me);
        mpierr      = MPI_Comm_size(mpicomm_grp, &mpisize);
        mpicomm     = mpicomm_grp;
      } else {
        me      = -1;
        mpisize = -1;
      }
    } else {
      if (mpirank==0) printf("Error. Incorrect np value: %d. Check number of procs\n", np);
      usage(mpirank);
    }
  } else {
    mpicomm = MPI_COMM_WORLD;
    me      = mpirank;
    mpisize = mpisizeall;
  }
  if (0 == mpirank) printf("No args = %d\n", argc);
  /* Set up MPI ready for IO */

  if (mpirank < np) {
    mpierr = MPI_Info_create(&mpiinfo);

  /*
   *   Following line can set the MPI Info object to include any performance hints
   *   that may be available in the MPI-IO implementation on the system.
   *   Please document any changes and hints used.
   */

#ifdef HINTS

/* Switch off data sieving */

    mpierr = MPI_Info_set(mpiinfo, "romio_ds_write", "disable");
    mpierr = MPI_Info_set(mpiinfo, "romio_ds_read",  "disable");
    mpierr = MPI_Info_set(mpiinfo, "romio_cb_write", "enable");
    mpierr = MPI_Info_set(mpiinfo, "romio_cb_read",  "enable");


#endif

    TIO_Set_Verbose(TIO_VERBOSE_ALL);


  /* Check mesh type and I/O mode */
  /* Remember: argc = narg+1 for executable */
    if (argc >= 8) {

      meshtype = NONE;
  
      if (strcasecmp("q", argv[1]) == 0) {
        meshtype = QUAD;
      }
      else if (strcasecmp("u", argv[1]) == 0) {
        meshtype = UNSTRUCT;
      }
      else if (strcasecmp("p", argv[1]) == 0) {
        meshtype = POINT;
      }
      else {
        mpierr  = MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
        if (0 == mpirank) printf("Error. Unknown mesh type\n"); 
        usage(mpirank);
      }

      mode = UNSET;

      if (strcasecmp("r", argv[7]) == 0) {
        mode = READ;
      }
      else if (strcasecmp("w", argv[7]) == 0) {
        mode = WRITE;
      }
      else if (strcasecmp("a", argv[7]) == 0) {
        mode = APPEND;
      }
      else if (strcasecmp("as", argv[7]) == 0) {
        mode = APP_ST;
      }
      else if (strcasecmp("aas", argv[7]) == 0) {
        mode = APP_APP_ST;
      }
      else if ((strcasecmp("ra", argv[7]) == 0) || (strcasecmp("ar", argv[7]) == 0)) {
        mode = READAPP;
      }
      else if ((strcasecmp("pr", argv[7]) == 0))  {
        mode = PLFS_READ;
      }
      else if ((strcasecmp("pw", argv[7]) == 0))  {
        mode = PLFS_WRITE;
      }
      else if ((strcasecmp("fpcw", argv[7]) == 0))  {
        mode = FPC_WRITE;
      }
      else if ((strcasecmp("fpcr", argv[7]) == 0))  {
        mode = FPC_READ;
      }
      else {
        mpierr  = MPI_Comm_rank(MPI_COMM_WORLD, &mpirank);
        if (0 == mpirank) printf("Error. Unknown r/w command\n");
        usage(mpirank);
      }

      /* Check if reads will by chunk, section or both */
  
      rflag = NEITHER;
      fflag = AUTO;
      sflag = SAME;
      if (argc >= 10) {
        if (strcasecmp("c", argv[9]) == 0) {
           rflag = CHUNK;
        }
        else if (strcasecmp("s", argv[9]) == 0) {
          rflag = SECTION;
          if (argc == 12) {
            fflag = USER;
            if ((mode==WRITE) || (mode==APPEND) || (mode==PLFS_WRITE) ) {
              if (mpirank==0)
                fprintf(stderr, "User defined filenames cannot be used with writes\n");
              usage(mpirank);
            }
            if (meshtype == QUAD) {
              if (mpirank==0)
                fprintf(stderr, "section reads not supported with Quad meshes\n");
              usage(mpirank);
            }
          }
        }
        else if (strcasecmp("ci", argv[9]) == 0) {
          rflag = CHUNK;
          sflag = INCREMENT;
        }
      }


      /* Run the tests */

      maxnum = atoi(argv[8]);

      for(outnum = 0; outnum < maxnum; outnum++) {
        main_single(argc, argv, outnum);
      }

    }       /*   if (argc > 9)  */
    else {
      if (0 == mpirank) printf("Error. Incorrect number of args (%d)\n", argc);
      usage(mpirank);
    }
	
    mpierr = MPI_Info_free(&mpiinfo);

  }   /* mpirank < np */

  mpierr = MPI_Finalize();	

  return (0);
}




long long int
filesize(const char *fname)
{
  int i;
  struct stat buf;

  i = stat(fname, &buf);

  return ((long long int)buf.st_size);
}



int
main_single(int argc, char *argv[],int outnum)
{

  static int   newfile = 1;

  char          fname[TIO_FILENAME_LEN];
  char          stname[TIO_STRLEN];
  char          str[TIO_STRLEN];

  TIO_t         err;
  TIO_File_t    fid;
  TIO_Object_t  stid, msid, matid, qid, vgid, vid, sid, nid;
  TIO_Object_t  nsectid, csectid;
  TIO_Data_t    graph_dtype, coord_dtype, mat_dtype, quant_dtype;
  TIO_Bool_t    ismixed = TIO_FALSE;
  TIO_Size_t    dims0D[1] = { 0 }, dims1D[1];


  /* Shape variables */

  int           *ncsh;
  TIO_Shape_t   *shapes;
  TIO_Size_t    nshape, ngstshape;


  /* Problem sizes */

  TIO_Size_t    n, gn, nghost, nreal, stride, nmix, nmcmp;
  TIO_Size_t    nread, npm;
  TIO_Size_t    nquant, nvar;
  TIO_Size_t    ner, nnr, n1, nchk;


  /* Data arrays */

  int           *r_mdat   = NULL, *mdat   = NULL;
  int           *r_mxmdat = NULL, *mxmdat = NULL;
  double        *r_qdat   = NULL, *qdat   = NULL;
  double        *r_mxqdat = NULL, *mxqdat = NULL;

  double        *r_xc     = NULL, *xc     = NULL;
  double        *r_yc     = NULL, *yc     = NULL;
  double        *r_zc     = NULL, *zc     = NULL;
  double        *whole_xc = NULL;
  double        *whole_yc = NULL;
  double        *whole_zc = NULL;

  TIO_Size_t    *r_conn   = NULL, *conn   = NULL;

  int           *r_ncomp  = NULL, *ncomp  = NULL;
  int           *r_ptr    = NULL, *ptr    = NULL;
  float         *r_vf     = NULL, *vf     = NULL;

  TIO_Size_t    *nl       = NULL, *nh     = NULL;
  TIO_Size_t    *pids     = NULL;
  TIO_Size_t    *r_nids   = NULL, *nids   = NULL;
  TIO_Size_t    *r_cids   = NULL, *cids   = NULL;

  int           *r_intvar = NULL, *intvar = NULL;
  double        *r_dblvar = NULL, *dblvar = NULL;
  int           r_v0idat,         v0idat;
  double        r_v0ddat,         v0ddat;

  int     i, j, p, k;
  double  t1, t2, t3, t4, tr1, tr2, tr3, tr4;



  /* Get time/date */

  time_t     now;
  char       date[TIO_STRLEN];
  struct tm  *ts;

  time(&now);
  ts = localtime(&now);
  strftime(date, sizeof(date), "%a %d-%m-%Y %H:%M", ts);


  /* process arguments */

  if (argc < 10 || argc > 12) {
    if (mpirank==0) printf("Error. Incorrect number of args (%d)\n", argc);
    usage(mpirank);
  }

  nread = atol(argv[2]);
  nreal = nread;

  if (sflag==INCREMENT) {
    nreal += (mpirank*100);
  }

  errno = 0;
  nmix  = atol(argv[3]);
  if (nmix > nreal) {
    if (me==0) printf("Error. No. mixed elements > No. real elements.\n");
    errno = 1;
  }
  else if (nmix > 0 && meshtype == POINT) {
    if (me==0) printf("Error. Point mesh cannot have mixed cells.\n");
    errno = 1;
  }
  if (errno >0 ) usage(mpirank);


  nghost= atol(argv[4]);
  if (nghost > 0) {
    if (me==0) printf("Error. IObench does not currently support ghost layers.\n");
    usage(mpirank);
  }

  n = nreal + nghost;

  nquant = atoi(argv[5]);

  nvar = atoi(argv[6]);

  gn  = (TIO_Size_t)(n * mpisize);


  if (mpirank == 0) {

    fprintf(stdout, "\niobench - version %s\n\n", version);
    if (meshtype == QUAD)
       fprintf(stdout, "quad mesh\n\n");
    else if (meshtype == UNSTRUCT)
       fprintf(stdout, "unstructured mesh\n\n");
    else
       fprintf(stdout, "point mesh\n\n");

    fprintf(stdout, "#0   Number of processes total             :  %lld\n", (long long int)( mpisizeall ) );
    fprintf(stdout, "#1   Number of processes written with      :  %lld\n", (long long int)( np ) );
    fprintf(stdout, "#2   Number of elements per proc           :  %lld\n", (long long int)( nreal ) );
    fprintf(stdout, "#3   Number of mixed elements per proc     :  %lld\n", (long long int)( nmix ) );
    fprintf(stdout, "#4   Number of ghost elements per proc     :  %lld\n", (long long int)( nghost ) );
    fprintf(stdout, "#5   Number of Quantities written/read     :  %lld\n", (long long int)( nquant ) );
    fprintf(stdout, "#6   Number of Variables written/read      :  %lld\n", (long long int)( nvar ) );
    fprintf(stdout, "#7   Mat/Quant bytes written/read per proc :  %lld\n", (long long int)
         ( (n * sizeof(int)) + (nquant * n * sizeof(double)) ) );
    fprintf(stdout, "#8   Number of elements in total           :  %lld\n", (long long int)( n * mpisize ) );
    fprintf(stdout, "#9   Mesh byte size in total               :  %lld\n",
         (long long int)( gn * sizeof(double) ));
    fprintf(stdout, "#10  Mat/Quant bytes written/read in total :  %lld\n",
         (long long int)( ((gn * sizeof(int)) + (nquant * gn * sizeof(double))) ) );
  }


  /* Allocate and initialise the data */

  tmalloc(mdat,  (sizeof(int)     * n   ), "Failed to allocate r_mdat"  );
  tmalloc(xc,    (sizeof(double)  * n   ), "Failed to allocate r_xc"    );
  tmalloc(yc,    (sizeof(double)  * n   ), "Failed to allocate r_yc"    );
  tmalloc(zc,    (sizeof(double)  * n   ), "Failed to allocate r_zc"    );

  if (nquant > 0)
    tmalloc(qdat,  (sizeof(double)  * n   ), "Failed to allocate r_qdat"  );

  /* Mixed data arrays */

  if (nmix > 0) {
     tmalloc(ptr,   (sizeof(int)     * nmix), "Failed to allocate r_ptr"   );
     tmalloc(ncomp, (sizeof(int)     * nmix), "Failed to allocate r_ncomp" );
     tmalloc(mxmdat,(sizeof(int)   *2* nmix), "Failed to allocate r_mxmdat");
     tmalloc(vf,    (sizeof(float) *2* nmix), "Failed to allocate r_vf"    );
     if (nquant > 0)
       tmalloc(mxqdat,(sizeof(double)*2* nmix), "Failed to allocate r_mxqdat");
  }


  /* Quad whole mesh arrays */

  if (meshtype == QUAD && mpirank == 0) {
     tmalloc(whole_xc,(sizeof(double)*(n/100)* mpisize +1 ),"Failed to alloc whole_xc");
     tmalloc(whole_yc,(sizeof(double) * 11), "Failed to allocate whole_yc");
     tmalloc(whole_zc,(sizeof(double) * 11), "Failed to allocate whole_zc");
  }


  /* Unstructured connectivity and ID arrays */

  if (meshtype == UNSTRUCT) {
    tmalloc(conn,  (sizeof(TIO_Size_t) * 8 * n   ), "Failed to allocate conn"  );
    tmalloc(nids,  (sizeof(TIO_Size_t) * n), "Failed to allocate r_nids" );
    tmalloc(cids,  (sizeof(TIO_Size_t) * n), "Failed to allocate r_cids" );
  }


  /* Point ID array */

  if (meshtype == POINT)
    tmalloc(pids,  (sizeof(TIO_Size_t) * n), "Failed to allocate pids" );


  /* Variables */

  if (nvar > 0) {
    dims1D[0] = n;
    if (mpirank ==0) {
      tmalloc(intvar, (sizeof(int)    * n), "Failed to allocate intvar" );
      tmalloc(dblvar, (sizeof(double) * n), "Failed to allocate dblvar" );
    }
  }

  /* Mesh Shapes */

  if (nghost > 0) {
    tmalloc(shapes, (sizeof(TIO_Shape_t) * 2), "Failed to allocate shapes");
    tmalloc(ncsh,   (sizeof(int) * 2), "Failed to allocate ncsh");
    shapes[0] = shapes[1] = TIO_SHAPE_HEX8;
    ncsh[0] = nreal;
    ncsh[1] = nghost;
  } else {
    tmalloc(shapes, (sizeof(TIO_Shape_t) * 1), "Failed to allocate shapes");
    tmalloc(ncsh,   (sizeof(int) * 1), "Failed to allocate ncsh");
    shapes[0] = TIO_SHAPE_HEX8;
    ncsh[0] = nreal;
  }

  /* Unmixed element data */

  p = mpirank;
  for (i=0; i<n; ++i) {
    mdat[i] = (int)(mpirank + 1);
    if (nquant > 0)
      qdat[i] = (double)(1000000 + mpirank + 1);
    if (meshtype != QUAD) {
      xc[i]   = 0.1 + (double) i;
      yc[i]   = 0.2 + (double) i;
      zc[i]   = 0.3 + (double) i;
    }
    if (meshtype==UNSTRUCT) {
      nids[i] = mpirank*n +i +1;
      cids[i] = mpirank*n +i +1;
      for (j=0; j<8; j++)
        conn[(i*8)+j] = i+1;
    }

    if (meshtype==POINT) {
      pids[i] = p;
      p += mpisize;
    }

    if (nvar > 0 && mpirank ==0) {
      intvar[i] = 1000002;
      dblvar[i] = (double)(1000003);
    }
  }

  /* Quad mesh coordinate data */

  if (meshtype == QUAD) {
    k = -1;

    for (j=0; j < mpisize; ++j) {
      for (i=0; i<(n/100); ++i) {
        if (j==0) {
          xc[i]   = 0.1 + (double) (i + (n/100)*mpirank);
        }
        ++k;
        if (mpirank == 0)
          whole_xc[k] = (double) k + 0.1;
      }
    }
    xc[(n/100)] = 0.1 + (double) (n/100)*(mpirank+1);
    if (mpirank == 0)
      whole_xc[k+1] = k + 1.1;

    for (i=0; i<11; ++i) {
      yc[i]       = 0.2 + (double) i;
      zc[i]       = 0.3 + (double) i;
      if (mpirank == 0) {
        whole_yc[i] = 0.2 + (double) i;
        whole_zc[i] = 0.3 + (double) i;
      }
    }

  }



  /* Mixed element data */

  if (nmix > 0) {
    ismixed = TIO_TRUE;
    stride = nreal/nmix;
    if (nreal%nmix > 0) stride+=1;
    j=0;
    for (i=0; i<nreal; i+=stride) {
      ptr[j] = 2*j+1;           /* Fills ptr with 1,3,5,7, etc */
      ncomp[j] = 2;
      mxmdat[ptr[j]-1] = 1;
      mxmdat[ptr[j]]   = 2;   /* Fills mxmdat with 1,2,1,2,1,2, etc */
      if (nquant > 0) {
        mxqdat[ptr[j]-1] = 1.0;
        mxqdat[ptr[j]]   = 2.0; /* Fills mxqdat with 1.0,2.0,1.0,2.0, etc */
      }
      vf[ptr[j]-1] = 0.5;
      vf[ptr[j]]   = 0.5;
      ++j;
      mdat[i] = -j;
    }
    if (j < nmix) {            /* Ensure correct number of mixed cells */
      ptr[j] = 2*j+1; 
      ncomp[j] = 2;
      mxmdat[ptr[j]-1] = 1;
      mxmdat[ptr[j]]   = 2;
      if (nquant > 0) {
        mxqdat[ptr[j]-1] = 1.0;
        mxqdat[ptr[j]]   = 2.0;
      }
      vf[ptr[j]-1] = 0.5;
      vf[ptr[j]]   = 0.5;
      ++j;
      mdat[nreal-1] = -j;
    }
  }



  /* Generate filename */

  if (fflag == AUTO) {
    if (mode == APPEND || mode == APP_APP_ST || mode == READAPP)
       sprintf(fname, "iob_d00_p%04d_e", mpisize);	
    else if ((mode == PLFS_WRITE) || (mode == PLFS_READ))
       sprintf(fname,"plfs:%d/iob_d%02d_p%04d_e",np,outnum,mpisize);
    else if ((mode == FPC_WRITE) || (mode == FPC_READ))
       sprintf(fname, "iob_d%02d_p%04d_r%02d_e", outnum, mpisizeall, mpirank);
    else
       sprintf(fname, "iob_d%02d_p%04d_e", outnum,mpisize);	

    strcat(fname, argv[2]);
    if (nmix > 0) {
      strcat(fname, "_mx");
      strcat(fname, argv[3]);
    }
    if (nghost > 0) {
      strcat(fname, "_g");
      strcat(fname, argv[4]);
    }
    if (nquant > 0) {
      strcat(fname, "_q");
      strcat(fname, argv[5]);
    }
    if (nvar > 0) {
      strcat(fname, "_v");
      strcat(fname, argv[6]);
    }
    strcat(fname, "_");
    strcat(fname, argv[1]);
    strcat(fname, ".h5");
  }
  else {
    sprintf(fname, argv[8]);
  }

  sprintf(stname, "state%04d", outnum);


  /**********************************************************************
   * Run the test(s) */


  /**********************************************************************
   * WRITES *
   **********/

  if ( (mode == WRITE) || (mode == APPEND) || (mode == PLFS_WRITE) ||
       (mode == FPC_WRITE) || (mode==APP_APP_ST) || (mode==APP_ST))  {

    mpierr = MPI_Barrier(mpicomm);

    t1 = MPI_Wtime();


    /* File */

    if ((mode == APPEND || mode==APP_APP_ST) && (newfile == 0)) {
       TIO_Call( TIO_Open(fname, &fid, TIO_ACC_READWRITE, "IObench", 
                     version, date, fname, mpicomm, mpiinfo, mpirank),
                     "Open File failed\n" );
    } else {
       TIO_Call( TIO_Create(fname, &fid, TIO_ACC_REPLACE, "IObench", 
                     version, date, fname, mpicomm, mpiinfo, mpirank),
                     "Create File failed\n" );
       newfile = 0;
    }



    /* State */
    TIO_Call( TIO_Create_State(fid, stname, &stid, 1, (TIO_Time_t)0.0, "us"),
	      "Create State failed\n" );



    /* Mesh - global nnodes, ncells, nshapes and nconnectivity given here */

    switch(meshtype) {
      case QUAD:

        tmalloc(nl, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nl\n");
        tmalloc(nh, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nh\n");
        nl[0] = 0;
        nh[0] = (n/100);

        for (p=1; p < mpisize; ++p) {
           nl[p] = nh[p-1];
           nh[p] = nl[p] + (n/100);
        }

        TIO_Call( TIO_Create_Mesh(fid, stid, "mesh", &msid, TIO_MESH_QUAD_COLINEAR,
                      TIO_COORD_CARTESIAN, TIO_FALSE, "group", (TIO_Size_t)1,
                      TIO_DATATYPE_NULL, TIO_DOUBLE, TIO_3D, (n*mpisize/100)+1, (TIO_Size_t)11,
                      (TIO_Size_t)11, (TIO_Size_t)NULL, (TIO_Size_t)mpisize, NULL, NULL,
                      NULL, NULL, NULL, NULL), "Create Mesh failed\n" );


        /* Set chunks */

        for (p=0; p<mpisize; p++) {
          TIO_Call( TIO_Set_Quad_Chunk(fid, msid, p, TIO_3D, nl[p], nh[p],
                      0, 10, 0, 10, nmix, nmix*2),
                      "Set UnstrMesh Chunk failed\n" );
        }

        /* Write whole mesh on PE 0 */

        if (mpirank == 0) 
           TIO_Call( TIO_Write_QuadMesh_All(fid, msid, TIO_DOUBLE,
                         whole_xc, whole_yc, whole_zc),
                         "Write UnstrMesh Chunk failed\n" );
        break;

      case UNSTRUCT:
        TIO_Call( TIO_Create_Mesh(fid, stid, "mesh", &msid, TIO_MESH_UNSTRUCT,
                      TIO_COORD_CARTESIAN, TIO_FALSE, "group", (TIO_Size_t)1,
                      TIO_SIZE_T, TIO_DOUBLE, TIO_3D, nreal*mpisize,
                      nreal*mpisize, mpisize, nreal*mpisize*8, mpisize, NULL,
                      NULL, NULL, NULL, NULL, NULL), "Create Mesh failed\n" );

        /* Set chunks - local nnodes, etc, given here*/

        nshape = 1;
        ngstshape = 0;
        if (nghost > 0) ngstshape = 1;
        for (p=0; p<mpisize; p++) {
          TIO_Call( TIO_Set_Unstr_Chunk(fid, msid, p, TIO_3D, nreal, nreal, nshape,
                      nreal*8, nghost, nghost, ngstshape, nghost*8, nmix, nmix*2),
                      "Set UnstrMesh Chunk failed\n" );
        }

        /* Write mesh chunks */

        TIO_Call( TIO_Write_UnstrMesh_Chunk(fid, msid, mpirank,
                      TIO_XFER, TIO_SIZE_T, TIO_DOUBLE,
                      nids, cids, shapes, ncsh, conn, xc, yc, zc),
	              "Write UnstrMesh Chunk failed\n" );
        break;

      default:    /* Point mesh is default */
        if (sflag==INCREMENT) {
           npm = 0;
           for(p=0; p< mpisize; p++) {
             npm += nread+(100*p);
           }
        }else{
           npm=nread*mpisize;
        }
        TIO_Call( TIO_Create_Mesh(fid,stid,"mesh",&msid,TIO_MESH_POINT,
                      TIO_COORD_CARTESIAN, TIO_FALSE, "atoms", 1, TIO_DATATYPE_NULL,
                      TIO_DOUBLE, TIO_3D, npm, 0, 0, 0, mpisize, 
                      NULL, NULL, NULL, NULL, NULL, NULL), "Create Mesh failed\n" );

        /* Set chunks */

        tmalloc(nl, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nl\n");
        tmalloc(nh, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nh\n");

        if (sflag==INCREMENT) {
           nl[0] = 0;
           nh[0] = nread-1;
           for (p=1; p < mpisize; p++) {
              nl[p] = nh[p-1] + 1;
              nh[p] = nread+(100*p) + nh[p-1];
           }
        } else {
           nl[0] =0;
           nh[0] =nread-1;
           for (p=1; p < mpisize; p++) {
              nl[p] = nh[p-1] + 1;
              nh[p] = nread+ nh[p-1];
           }
        }

        /* Write the mesh */

        TIO_Call( TIO_Write_PointMesh_Chunk( fid, msid, mpirank, 
                      TIO_XFER, TIO_DOUBLE, xc, yc, zc),
                      "Write PointMesh chunk failed\n");

    }
 

    /* Material - inherits and uses same chunks as in the mesh */

    switch(meshtype) {
      case QUAD:
        TIO_Call( TIO_Create_Material(fid, msid, "Material", &matid, TIO_INT,
                      mpisize, TIO_GHOSTS_NONE, ismixed, TIO_INT,
                      TIO_INT, TIO_FLOAT),
                      "Create Material failed\n" );

        TIO_Call( TIO_Write_QuadMaterial_Chunk(fid, matid, mpirank,
                      TIO_XFER, TIO_INT, mdat,
                      TIO_INT, TIO_INT, TIO_FLOAT, ncomp,
                      ptr, mxmdat, vf),
	              "Write UnstrMaterial Chunk failed\n" );
        break;

      case UNSTRUCT:
        TIO_Call( TIO_Create_Material(fid, msid, "Material", &matid, TIO_INT,
                      mpisize, TIO_GHOSTS_ALL, ismixed, TIO_INT,
                      TIO_INT, TIO_FLOAT),
                      "Create Material failed\n" );

        TIO_Call( TIO_Write_UnstrMaterial_Chunk(fid, matid, mpirank,
                      TIO_XFER, TIO_INT, mdat,
                      TIO_INT, TIO_INT, TIO_FLOAT, ncomp,
                      ptr, mxmdat, vf),
                      "Write UnstrMaterial Chunk failed\n" );

        break;

      default:
        TIO_Call( TIO_Create_Material(fid, msid, "Material", &matid, TIO_INT,
                      mpisize, TIO_GHOSTS_ALL, ismixed, TIO_INT,
                      TIO_INT, TIO_DOUBLE),
                      "Create Material failed\n" );

        TIO_Call( TIO_Write_PointMaterial_Chunk(fid, matid, mpirank, 
                      TIO_XFER, TIO_INT, mdat),
                      "Write PointMaterial Chunk failed\n" );

    }

    TIO_Call( TIO_Close_Material(fid, matid), "Close Material failed\n" );


    /* Quantity - again, inherits and uses same chunks as in the mesh */


    for (i=0; i < nquant; i++)
    {
      switch(meshtype) {
        case QUAD:
           sprintf(str, "Quant_%d", i);
           TIO_Call( TIO_Create_Quant(fid, msid, str, &qid, TIO_DOUBLE,
                         TIO_CENTRE_CELL, TIO_GHOSTS_NONE, ismixed, "qunits"),
                         "Create Quant failed\n" );

           TIO_Call( TIO_Write_QuadQuant_Chunk(fid, qid, mpirank,
                         TIO_XFER, TIO_DOUBLE, qdat, mxqdat),
                         "Write QuadQuant Chunk failed\n" );

           TIO_Call( TIO_Close_Quant(fid, qid),      "Close Quant failed\n" );

           /* update data for next iteration */
           for (j=0; j<n; j++) qdat[j] += (double) 1000000;
        break;

        case UNSTRUCT:
           sprintf(str, "Quant_%d", i);
           TIO_Call( TIO_Create_Quant(fid, msid, str, &qid, TIO_DOUBLE,
                         TIO_CENTRE_CELL, TIO_GHOSTS_ALL,ismixed, "qunits"),
                         "Create Quant failed\n" );

           TIO_Call( TIO_Write_UnstrQuant_Chunk(fid, qid, mpirank,
                         TIO_XFER, TIO_DOUBLE, qdat, mxqdat),
                         "Write UnstrQuant Chunk failed\n" );

           TIO_Call( TIO_Close_Quant(fid, qid),      "Close Quant failed\n" );

           /* update data for next iteration */
           for (j=0; j<n; j++) qdat[j] += (double) 1000000;

        break;

        default:
           sprintf(str, "Quant_%d", i);
           TIO_Call( TIO_Create_Quant(fid, msid, str, &qid, TIO_DOUBLE, 
                         TIO_CENTRE_NODE,TIO_GHOSTS_ALL, TIO_FALSE, "qunits"),
                         "Create Quant failed\n" );

           TIO_Call( TIO_Write_PointQuant_Chunk(fid, qid, mpirank, 
                         TIO_XFER, TIO_DOUBLE, qdat),
                         "Write UnstrQuant Chunk failed\n" );

           TIO_Call( TIO_Close_Quant(fid, qid),      "Close Quant failed\n" );

           /* update data for next iteration */
           for (j=0; j<n; j++) qdat[j] += (double) 1000000;

      }

    }


    /* Variables (Created collectively, but only written on Proc 0)
       One 0D and one 1D double and int variable written for each nvar
       (Four in total) */

    if (nvar > 0) {

      /* Create a vargroup to store the variables in */

      TIO_Call( TIO_Create_Vargroup(fid, stid, "Variables", &vgid),
                    "Create Vargroup failed\n" );

      for (i=0; i < nvar; ++i) {

        /* 0D int variable */

        sprintf(str, "Var_int_0D_%d", i);
        TIO_Call( TIO_Create_Variable(fid, vgid, str, &vid, TIO_INT, TIO_0D,
                      dims0D, NULL), "Create 0D int Variable failed\n" );

        if (mpirank==0)
          TIO_Call( TIO_Write_Variable(fid, vid, TIO_INT, &i),
                        "Write 0D int Variable failed\n" );

        TIO_Call( TIO_Close_Variable(fid, vid),
                      "Close 0D int Variable failed\n" );


        /* 0D double variable */

        sprintf(str, "Var_dbl_0D_%d", i);
        v0ddat = (double) i;
        TIO_Call( TIO_Create_Variable(fid, vgid, str, &vid, TIO_DOUBLE, TIO_0D,
                      dims0D, NULL), "Create 0D double Variable failed\n" );

        if (mpirank==0)
          TIO_Call( TIO_Write_Variable(fid, vid, TIO_DOUBLE, &v0ddat),
                        "Write 0D double Variable failed\n" );

        TIO_Call( TIO_Close_Variable(fid, vid),
                      "Close 0D double Variable failed\n" );


        /* 1D int variable */

        sprintf(str, "Var_int_1D_%d", i);
        TIO_Call( TIO_Create_Variable(fid, vgid, str, &vid, TIO_INT, TIO_1D,
                      dims1D, NULL), "Create 1D int Variable failed\n" );

        if (mpirank==0)
          TIO_Call( TIO_Write_Variable(fid, vid, TIO_INT, intvar),
                        "Write 1D int Variable failed\n" );

        TIO_Call( TIO_Close_Variable(fid, vid),
                      "Close 1D int Variable failed\n" );


        /* 1D double variable */

        sprintf(str, "Var_dbl_1D_%d", i);
        TIO_Call( TIO_Create_Variable(fid, vgid, str, &vid, TIO_DOUBLE, TIO_1D,
                      dims1D, NULL), "Create 1D double Variable failed\n" );

        if (mpirank==0)
          TIO_Call( TIO_Write_Variable(fid, vid, TIO_DOUBLE, dblvar),
                        "Write 1D double Variable failed\n" );

        TIO_Call( TIO_Close_Variable(fid, vid),
                      "Close 1D double Variable failed\n" );

        if (mpirank ==0) {
          /* update data for next iteration */
          for (j=0; j<n; j++) {
            intvar[j] += 1000000;
            dblvar[j] += (double)1000000;
          }
        }
      }

      TIO_Call( TIO_Close_Vargroup(fid, vgid), "Close Vargroup failed\n" );

    }


    /* Close up */

    mpierr = MPI_Barrier(mpicomm);
    TIO_Call( TIO_Close_Mesh(fid, msid),      "Close Mesh failed\n" );
    TIO_Call( TIO_Close_State(fid, stid),     "Close State failed\n" );
    TIO_Call( TIO_Close(fid),                 "Close File failed\n");

    if (mode == APP_ST || mode == APP_APP_ST) {

      TIO_Call( TIO_Open(fname, &fid, TIO_ACC_READWRITE, NULL, NULL, 
                    NULL, NULL, mpicomm, mpiinfo, mpirank),
                    "Open File failed\n" );


      /* State */

      TIO_Call( TIO_Open_State(fid, stname, &stid, NULL, NULL, NULL),
                   "Open State failed\n" );


      if (nvar > 0) {

        /* Create a vargroup to store the variables in */

        TIO_Call( TIO_Create_Vargroup(fid, stid, "Variables2", &vgid),
                      "Create Vargroup2 failed\n" );

        for (i=0; i < nvar; ++i) {

          /* 0D int variable */
          j=i+nvar;
          sprintf(str, "Var_int_0D_%d", j);
          TIO_Call( TIO_Create_Variable(fid, vgid, str, &vid, TIO_INT, TIO_0D,
                        dims0D, NULL), "Create 0D int Variable failed\n" );

          if (mpirank==0)
            TIO_Call( TIO_Write_Variable(fid, vid, TIO_INT, &i),
                          "Write 0D int Variable failed\n" );

          TIO_Call( TIO_Close_Variable(fid, vid),
                        "Close 0D int Variable failed\n" );

        }

        TIO_Call( TIO_Close_Vargroup(fid, vgid), "Close Vargroup2 failed\n" );

      }

      TIO_Call( TIO_Close_State(fid, stid), "Close State failed\n" );
      TIO_Call( TIO_Close(fid),             "Close File failed\n");
    }

    t2 = MPI_Wtime();

    t3 = t2-t1;
    mpierr = MPI_Reduce(&t3, &tr3, 1, MPI_DOUBLE, MPI_MAX, 0, mpicomm);

    if (mpirank == 0) {
      if (mode == FPC_WRITE) {
        fprintf(stdout, "#13  TyphonIO file per core size (bytes)   :  %lld\n\n",
               (long long int)filesize(fname));
      } else {
        fprintf(stdout, "#13  TyphonIO file size (bytes)            :  %lld\n\n",
               (long long int)filesize(fname));
      }
      fprintf(stdout, "#14  WRITE        -> Total   Max time taken (per proc) : %f\n", tr3);
    }

    if (meshtype == POINT || meshtype == QUAD) {
       free(nh);
       free(nl);
    }

  }

#ifdef DEBUG
  /* Reinitialise qdat reference data */
  for (i=0; i<n; i++) {
    if (nquant > 0)
      qdat[i] = (double)(1000000 + mpirank + 1);
    if (nvar > 0 && mpirank ==0) {
      intvar[i] = 1000002;
      dblvar[i] = (double)(1000003);
    }
  }
#endif


  if ( (mode == READ) || (mode == READAPP) || (mode == PLFS_READ) ||
       (mode == FPC_READ) || (mode == FPC_READ))  {

    /*
     *   READ
     */

    tmalloc(r_mdat, (sizeof(int)     * n), "Failed to allocate r_mdat");
    if (nquant > 0)
       tmalloc(r_qdat, (sizeof(double)  * n), "Failed to allocate r_qdat");
       tmalloc(r_xc,   (sizeof(double)  * n), "Failed to allocate r_xc");
       tmalloc(r_yc,   (sizeof(double)  * n), "Failed to allocate r_yc");
       tmalloc(r_zc,   (sizeof(double)  * n), "Failed to allocate r_zc");
    if (nmix > 0) {
       tmalloc(r_ptr,   (sizeof(int)     * nmix), "Failed to allocate r_ptr"   );
       tmalloc(r_ncomp, (sizeof(int)     * nmix), "Failed to allocate r_ncomp" );
       tmalloc(r_mxmdat,(sizeof(int)   *2* nmix), "Failed to allocate r_mxmdat");
       tmalloc(r_vf,    (sizeof(float) *2* nmix), "Failed to allocate r_vf"    );
       if (nquant > 0)
         tmalloc(r_mxqdat,(sizeof(double)*2* nmix), "Failed to allocate r_mxqdat");
    }
    if (meshtype == UNSTRUCT) {
       tmalloc(r_conn, (sizeof(TIO_Size_t) * 8 * n), "Failed to allocate r_conn");
       tmalloc(r_nids, (sizeof(TIO_Size_t)     * n), "Failed to allocate r_nids");
       tmalloc(r_cids, (sizeof(TIO_Size_t)     * n), "Failed to allocate r_cids");
    }

    /* Variables */

    if (nvar > 0) {
      if (mpirank ==0) {
        tmalloc(r_intvar, (sizeof(int)    * n), "Failed to allocate r_intvar" );
        tmalloc(r_dblvar, (sizeof(double) * n), "Failed to allocate r_dblvar" );
      }
    }


    /**********************************************************************
     * CHUNK READS *
     ***************/

    if (rflag == CHUNK) {

      mpierr = MPI_Barrier(mpicomm);
      t1 = MPI_Wtime();

      /* File */

      TIO_Call( TIO_Open(fname, &fid, TIO_ACC_READONLY, NULL, NULL, 
                    NULL, NULL, mpicomm, mpiinfo, mpirank),
                    "Open File failed\n" );


      /* State */

      TIO_Call( TIO_Open_State(fid, stname, &stid, NULL, NULL, NULL),
                   "Open State failed\n" );



      /* Mesh */

      TIO_Call( TIO_Open_Mesh(fid, stid, "mesh", &msid, NULL, NULL,
                    NULL, NULL, NULL, &graph_dtype, &coord_dtype,
                    NULL, &n1, NULL, NULL, NULL, NULL, NULL,
                    NULL, NULL, NULL, NULL, NULL),
                    "Open Mesh failed\n" );

/*  Test for when number of sections != No of chunks - Unfinished.  */
      if (meshtype == QUAD) {
        if ( (n1-1)*100 != n*mpisize) {
           if (me==0)
             printf("Error. User defined global no. nodes does not match value in file\n");
           usage(mpirank);
        }
      } else {
        if (n1 != n*mpisize) {
          if (me==0)
            printf("Error. User defined global no. nodes does not match value in file\n");
          usage(mpirank);
        }
      }


      /* Material */

      TIO_Call( TIO_Open_Material(fid, msid, "Material", &matid, &mat_dtype,
                    NULL, NULL, NULL, NULL, NULL, NULL),
                    "Open Material failed\n" );


      /* Mesh & Material Chunks */

      switch(meshtype) {
        case QUAD:

          TIO_Call( TIO_Read_QuadMesh_Chunk(fid, msid, mpirank,
                        TIO_XFER, coord_dtype,
                        (TIO_Size_t)NULL, NULL, (TIO_Size_t*)NULL, 
                        (TIO_Size_t*)NULL, (TIO_Size_t*)NULL, r_xc, r_yc, r_zc ),
                        "Read QuadMesh Chunk failed\n" );

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_QuadMaterial_Chunk(fid, matid, mpirank,
                        TIO_XFER, mat_dtype, (TIO_Size_t)NULL,
                        NULL, (TIO_Size_t*)NULL, (TIO_Size_t*)NULL,
                        (TIO_Size_t*)NULL, r_mdat, TIO_DATATYPE_NULL, 
                        TIO_DATATYPE_NULL, TIO_DATATYPE_NULL, 
                        &nmix, &nmcmp, NULL, NULL, NULL, NULL),
                       "Read QuadMaterial Chunk (1) failed\n" );
          if (nmcmp > 0) {
            if (nmix == 0) {
              if (me==0) printf("Error. Mixed data specified, but none found.\n");
              usage(mpirank);
            }
            free(r_mxmdat);
            tmalloc(r_mxmdat,(sizeof(int) * nmcmp), "Failed to allocate r_mxmdat");

            TIO_Call( TIO_Read_QuadMaterial_Chunk(fid, matid, mpirank,
                          TIO_XFER, mat_dtype, (TIO_Size_t)NULL, 
                          NULL, (TIO_Size_t*)NULL, (TIO_Size_t*)NULL, 
                          (TIO_Size_t*)NULL, mdat, TIO_INT, TIO_INT, TIO_FLOAT,
                          &nmix, &nmcmp, r_ncomp, r_ptr, r_mxmdat, r_vf),
                          "Read QuadMaterial Chunk (2) failed\n" );
          }

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , (n/100)+1);
          chkdbl("yc",     yc, r_yc  , 11);
          chkdbl("zc",     zc, r_zc  , 11);
          chkint("mdat", mdat, r_mdat, n); 
#endif

          break;

        case UNSTRUCT:
          TIO_Call( TIO_Read_UnstrMesh_Chunk(fid, msid, mpirank,
                        TIO_XFER, TIO_SIZE_T,coord_dtype,
                        (TIO_Size_t)NULL, NULL, (TIO_Size_t*)NULL, (TIO_Size_t*)NULL, 
                        (TIO_Size_t*)NULL, (TIO_Size_t*)NULL, r_nids,
                        r_cids, NULL, NULL, r_conn, r_xc, r_yc, r_zc ),
                        "Read UnstrMesh Chunk failed\n" );

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_UnstrMaterial_Chunk(fid, matid, mpirank,
                        TIO_XFER, mat_dtype,
                        (TIO_Size_t)NULL, (TIO_Size_t*)NULL, r_mdat, TIO_DATATYPE_NULL, 
                        TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                        &nmix, &nmcmp, NULL, NULL, NULL, NULL),
                        "Read UnstrMaterial Chunk (1) failed\n" );

          if (nmcmp > 0) {
            if (nmix == 0) {
              if (me==0) printf("Error. Mixed data specified, but none found.\n");
              usage(mpirank);
            }
            free(r_mxmdat);
            tmalloc(r_mxmdat,(sizeof(int) * nmcmp), "Failed to allocate r_mxmdat");

            TIO_Call( TIO_Read_UnstrMaterial_Chunk(fid, matid, mpirank,
                          TIO_XFER, mat_dtype, (TIO_Size_t)NULL, NULL,
                          r_mdat, TIO_INT, TIO_INT, TIO_FLOAT,
                          &nmix, &nmcmp, r_ncomp, r_ptr, r_mxmdat, r_vf),
                          "Read UnstrMaterial Chunk (2) failed\n" );
          }

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , n);
          chkdbl("yc",     yc, r_yc  , n);
          chkdbl("zc",     zc, r_zc  , n);
          chkint("mdat", mdat, r_mdat, n); 
          chkint("conn", conn, r_conn, 8*n);
          chkint("nids", nids, r_nids, n);
          chkint("cids", cids, r_cids, n); 
#endif

          break;

        default:
          TIO_Call( TIO_Read_PointMesh_Chunk(fid, msid, mpirank,
                        TIO_XFER, coord_dtype, (TIO_Size_t)NULL, NULL,
                         NULL, r_xc, r_yc, r_zc ), "Read PointMesh Chunk failed\n");

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_PointMaterial_Chunk(fid, matid, mpirank,
                        TIO_XFER, mat_dtype, (TIO_Size_t)NULL, NULL, r_mdat),
                        "Read PointMaterial Chunk failed\n" );

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , n);
          chkdbl("yc",     yc, r_yc  , n);
          chkdbl("zc",     zc, r_zc  , n);
          chkint("mdat", mdat, r_mdat, n); 
#endif
      }

      /* Quantities */


      for (i=0; i < nquant; i++) 
      {

         sprintf(str, "Quant_%d", i);

         TIO_Call( TIO_Open_Quant(fid, msid, str, &qid, &quant_dtype,
                       NULL, NULL, NULL, NULL), "Open Quant failed\n" );


         switch(meshtype) {
           case QUAD:
             TIO_Call( TIO_Read_QuadQuant_Chunk(fid, qid, mpirank,
                           TIO_XFER, quant_dtype, (TIO_Size_t)NULL,
                           NULL, NULL, NULL, NULL, r_qdat, &nmcmp, NULL),
                           "Read UnstrQuant Chunk failed\n" );

#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

             if (nmcmp > 0) {
               free(r_mxqdat);
               tmalloc(r_mxqdat,(sizeof(double)* nmcmp),
                  "Failed to allocate r_mxqdat");

               TIO_Call( TIO_Read_QuadQuant_Chunk(fid, qid, mpirank,
                             TIO_XFER, quant_dtype, (TIO_Size_t)NULL,
                             NULL, NULL, NULL, NULL, r_qdat, NULL, r_mxqdat),
                             "Read UnstrQuant Chunk failed\n" );
             }

             break;

           case UNSTRUCT:
             TIO_Call( TIO_Read_UnstrQuant_Chunk(fid, qid, mpirank,
                           TIO_XFER, quant_dtype,
                           (TIO_Size_t)NULL, NULL, r_qdat, &nmcmp, NULL),
                           "Read UnstrQuant Chunk failed\n" );

#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

             if (nmcmp > 0) {
               free(r_mxqdat);
               tmalloc(r_mxqdat,(sizeof(double)* nmcmp),
                  "Failed to allocate r_mxqdat");

               TIO_Call( TIO_Read_UnstrQuant_Chunk(fid, qid, mpirank,
                             TIO_XFER, quant_dtype,
                             (TIO_Size_t)NULL, NULL, r_qdat, NULL, r_mxqdat),
                             "Read UnstrQuant Chunk failed\n" );
             }

             break;

           default:
             TIO_Call( TIO_Read_PointQuant_Chunk(fid, qid, mpirank,
                           TIO_XFER, quant_dtype,
                           TIO_GHOSTS_NONE, NULL, r_qdat),
                           "Read UnstrQuant Chunk failed\n" );
#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

         }

         TIO_Call( TIO_Close_Quant(fid, qid), "Close Quant failed\n" );

      }

      /* Variables */

      if (nvar > 0) {

        TIO_Call( TIO_Open_Vargroup( fid, stid, "Variables", &vgid),
                      "Open Vargroup failed\n" );

        for (i=0; i < nvar; ++i) {

        /* 0D int variable */

          sprintf(str, "Var_int_0D_%d", i);
          TIO_Call( TIO_Open_Variable(fid, vgid, str, &vid, NULL, NULL,
                        NULL, NULL), "Create 0D int Variable failed\n" );

          v0idat = i;


          if (mpirank==0)
            TIO_Call( TIO_Read_Variable(fid, vid, TIO_INT, &r_v0idat),
                        "Read 0D int Variable failed\n" );

          TIO_Call( TIO_Close_Variable(fid, vid),
                        "Close 0D int Variable failed\n" );


          /* 0D double variable */

          sprintf(str, "Var_dbl_0D_%d", i);
          v0ddat = (double) i;
          TIO_Call( TIO_Open_Variable(fid, vgid, str, &vid, NULL, NULL,
                        NULL, NULL), "Create 0D double Variable failed\n" );

          if (mpirank==0)
            TIO_Call( TIO_Read_Variable(fid, vid, TIO_DOUBLE, &r_v0ddat),
                          "Read 0D double Variable failed\n" );

          TIO_Call( TIO_Close_Variable(fid, vid),
                        "Close 0D double Variable failed\n" );


          /* 1D int variable */

          sprintf(str, "Var_int_1D_%d", i);
          TIO_Call( TIO_Open_Variable(fid, vgid, str, &vid, NULL, NULL,
                        NULL, NULL), "Create 1D int Variable failed\n" );

          if (mpirank==0)
            TIO_Call( TIO_Read_Variable(fid, vid, TIO_INT, r_intvar),
                          "Read 1D int Variable failed\n" );

          TIO_Call( TIO_Close_Variable(fid, vid),
                        "Close 1D int Variable failed\n" );


          /* 1D double variable */

          sprintf(str, "Var_dbl_1D_%d", i);
          TIO_Call( TIO_Open_Variable(fid, vgid, str, &vid, NULL, NULL,
                        NULL, NULL), "Create 1D double Variable failed\n" );

          if (mpirank==0)
            TIO_Call( TIO_Read_Variable(fid, vid, TIO_DOUBLE, r_dblvar),
                          "Read 1D double Variable failed\n" );

          TIO_Call( TIO_Close_Variable(fid, vid),
                        "Close 1D double Variable failed\n" );


#ifdef DEBUG
          if (mpirank ==0) {
            chkint("v0Didat", &v0idat, &r_v0idat, 1);
            chkdbl("v0Dddat", &v0ddat, &r_v0ddat, 1);
            chkint("intvar",  intvar,  r_intvar, n);
            chkdbl("dblvar",  dblvar,  r_dblvar, n); 

            /* update data for next iteration */
            for (j=0; j<n; j++) {
              intvar[j] += 1000000;
              dblvar[j] += (double)1000000;
            }

          }
#endif

       }

        TIO_Call( TIO_Close_Vargroup(fid, vgid), "Close Vargroup failed\n" );

      }

      t3 = MPI_Wtime();


      /* Close up */

      TIO_Call( TIO_Close_Material(fid, matid), "Close Material failed\n" );


      TIO_Call( TIO_Close_Mesh(fid, msid),      "Close Mesh failed\n" );


      TIO_Call( TIO_Close_State(fid, stid),     "Close State failed\n" );

      TIO_Call( TIO_Close(fid),                 "Close File failed\n");


      t4 = MPI_Wtime();

      t3=t3-t2;
      t4=t4-t1;

      mpierr = MPI_Reduce(&t3, &tr3, 1, MPI_DOUBLE, MPI_MIN, 0, mpicomm);
      mpierr = MPI_Reduce(&t4, &tr4, 1, MPI_DOUBLE, MPI_MAX, 0, mpicomm);

      if (mpirank == 0) {
        fprintf(stdout, "\n#13  READ Chunk   -> Mat/Quant Max time taken (per proc) : %f\n", tr3);
        if (mode == FPC_READ) {
          fprintf(stdout, "#14  READ Chunk per core  -> Total   Max time taken      : %f\n", tr4);
        } else {
          fprintf(stdout, "#14  READ Chunk   -> Total     Max time taken (per proc) : %f\n", tr4);
        }
      }
      mpierr = MPI_Barrier(mpicomm);

#ifdef DEBUG
      /* Reinitialise qdat reference data */
    for (i=0; i<n; i++) {
      if (nquant > 0)
        qdat[i] = (double)(1000000 + mpirank + 1);
      if (nvar > 0 && mpirank ==0) {
        intvar[i] = 1000002;
        dblvar[i] = (double)(1000003);
      }
    }

#endif


    }  /* rflag == CHUNK */


    /**********************************************************************
     * SECTION READS *
     *****************/

    if (rflag == SECTION) {

      if (meshtype==UNSTRUCT) {
        p = mpirank+1;
        for (i=0; i < n; ++i) {
          nids[i] = p;
          cids[i] = p;
          p += mpisize;
        }
      }

      mpierr = MPI_Barrier(mpicomm);
      t1 = MPI_Wtime();

      /* File */

      TIO_Call( TIO_Open(fname, &fid, TIO_ACC_READONLY, NULL, NULL, NULL, NULL,
                    mpicomm, mpiinfo, mpirank), "Open File failed\n" );


      /* State */

      TIO_Call( TIO_Open_State(fid, stname, &stid, NULL, NULL, NULL),
                    "Open State failed\n" );



      /* Mesh and Material*/

      TIO_Call( TIO_Open_Mesh(fid, stid, "mesh", &msid, NULL, NULL,
                    NULL, NULL, NULL, &graph_dtype, &coord_dtype,
                    NULL, &n1, NULL, NULL, NULL,
                    NULL, NULL, NULL, NULL, NULL, NULL, NULL),
                    "Open Mesh failed\n" );

      if (meshtype == QUAD) {
        if ( (n1-1)*100 != n*mpisize) {
           if (me==0)
             printf("Error. User defined global no. nodes does not match value in file\n");
           usage(mpirank);
        }
      } else {
        if (n1 != n*mpisize) {
          if (me==0)
            printf("Error. User defined global no. nodes does not match value in file\n");
          usage(mpirank);
        }
      }


      TIO_Call( TIO_Open_Material(fid, msid, "Material", &matid,
                    &mat_dtype, NULL, NULL, NULL, NULL, NULL, NULL),
                    "Open Material failed\n" );


    /* Mesh and Material Chunks */

      switch(meshtype) {

        case QUAD:
          tmalloc(nl, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nl\n");
          tmalloc(nh, (sizeof(TIO_Size_t) * mpisize), "Failed to allocate nh\n");
          nl[0] = 0;
          nh[0] = (n/100);

          for (p=1; p < mpisize; ++p) {
             nl[p] = nh[p-1];
             nh[p] = nl[p] + (n/100);
          }
          TIO_Call( TIO_Set_Quad_Section(fid, msid, &nsectid, TIO_3D, nl[me], nh[me],
                        (TIO_Size_t)0, (TIO_Size_t)10, (TIO_Size_t)0, (TIO_Size_t)10),
                        "Set Quad section failed\n" );

          TIO_Call( TIO_Read_QuadMesh_Section(fid, msid, nsectid,
                        TIO_XFER, coord_dtype,
                        NULL, NULL, NULL, NULL, r_xc, r_yc, r_zc ),
                        "Read QuadMesh Section failed\n" );

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_QuadMaterial_Section(fid, matid, nsectid,
                        TIO_XFER, mat_dtype, NULL, (TIO_Size_t*)NULL,
                        (TIO_Size_t*)NULL,  (TIO_Size_t*)NULL,  r_mdat, TIO_DATATYPE_NULL, 
                        TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                        &nmix, &nmcmp, NULL, NULL, NULL, NULL),
                        "Read QuadMaterial Section (1) failed\n" );

          if (nmcmp > 0) {
            if (nmix == 0) {
              if (me==0) printf("Error. Mixed data specified, but none found.\n");
              usage(mpirank);
            }
            free(mxmdat);
            tmalloc(mxmdat,(sizeof(int) * nmcmp), "Failed to allocate mxmdat");

            TIO_Call( TIO_Read_QuadMaterial_Section(fid, matid, nsectid,
                          TIO_XFER, mat_dtype, NULL, NULL,
                          NULL, NULL, r_mdat, TIO_INT, TIO_INT, TIO_FLOAT,
                          &nmix, &nmcmp, r_ncomp, r_ptr, r_mxmdat, r_vf),
                          "Read QuadMaterial Section (2) failed\n" );
          }

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , (n/100)+1);
          chkdbl("yc",     yc, r_yc  , 11);
          chkdbl("zc",     zc, r_zc  , 11);
          chkint("mdat", mdat, r_mdat, n); 
#endif

          break;

        case UNSTRUCT:
          nnr = (TIO_Size_t) n;
          ner = (TIO_Size_t) n;


          TIO_Call( TIO_Set_Unstr_Section(fid,msid,&csectid,TIO_CENTRE_CELL,ner,cids),
                        "Set Unstr cell section failed\n");
          TIO_Call( TIO_Set_Unstr_Section(fid,msid,&nsectid,TIO_CENTRE_NODE,nnr,nids),
                        "Set Unstr cell section failed\n");

          TIO_Call( TIO_Read_UnstrMesh_Section(fid, msid, nsectid,
                        TIO_XFER, coord_dtype,
                        NULL, NULL, r_xc, r_yc, r_zc ),
                        "Read UnstrMesh Section failed\n" );

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_UnstrMaterial_Section(fid, matid, csectid,
                        TIO_XFER, mat_dtype,
                        NULL,  r_mdat, TIO_DATATYPE_NULL, TIO_DATATYPE_NULL,
                        TIO_DATATYPE_NULL,
                        &nmix, &nmcmp, NULL, NULL, NULL, NULL),
                        "Read UnstrMaterial Section (1) failed\n" );

          if (nmcmp > 0) {
            if (nmix == 0) {
              if (me==0) printf("Error. Mixed data specified, but none found.\n");
              usage(mpirank);
            }
            free(r_mxmdat);
            tmalloc(r_mxmdat,(sizeof(int) * nmcmp), "Failed to allocate r_mxmdat");

            TIO_Call( TIO_Read_UnstrMaterial_Section(fid, matid, csectid,
                          TIO_XFER, mat_dtype,
                          NULL, r_mdat, TIO_INT, TIO_INT, TIO_FLOAT,
                          &nmix, &nmcmp, r_ncomp, r_ptr, r_mxmdat, r_vf),
                          "Read UnstrMaterial Section (2) failed\n" );
          }

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , n);
          chkdbl("yc",     yc, r_yc  , n);
          chkdbl("zc",     zc, r_zc  , n);
          chkint("mdat", mdat, r_mdat, n); 
#endif
          break;

        default:
          TIO_Call( TIO_Set_Point_Section(fid,msid,&nsectid, (TIO_Size_t)n, pids),
                        "Set Unstr cell section failed\n");

          TIO_Call( TIO_Read_PointMesh_Section(fid, msid, nsectid,
                        TIO_XFER, coord_dtype, NULL,
                        NULL, r_xc, r_yc, r_zc ),
                        "Read PointMesh Section failed\n");

          t2 = MPI_Wtime();
          TIO_Call( TIO_Read_PointMaterial_Section(fid, matid, nsectid,
                        TIO_XFER, mat_dtype, NULL, r_mdat),
                        "Read PointMaterial Section failed\n" );

#ifdef DEBUG
          chkdbl("xc",     xc, r_xc  , n);
          chkdbl("yc",     yc, r_yc  , n);
          chkdbl("zc",     zc, r_zc  , n);
          chkint("mdat", mdat, r_mdat, n); 
#endif

      }



      /* Quantity */

      for (i=0; i < nquant; i++)
      {
         sprintf(str, "Quant_%d", i);

         TIO_Call( TIO_Open_Quant(fid, msid, str, &qid, &quant_dtype,
                       NULL, NULL, NULL, NULL), "Open Quant failed\n" );


         switch(meshtype) {
           case QUAD:
             TIO_Call( TIO_Read_QuadQuant_Section(fid, qid, nsectid,
                           TIO_XFER, quant_dtype, NULL, 
                           NULL, NULL, NULL, r_qdat, &nmcmp, NULL),
                           "Read UnstrQuant Section failed\n" );

#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

             if (nmcmp > 0) {
               free(r_mxqdat);
               tmalloc(r_mxqdat,(sizeof(double)* nmcmp), 
                       "Failed to allocate r_mxqdat");

               TIO_Call( TIO_Read_QuadQuant_Section(fid, qid, nsectid,
                             TIO_XFER, quant_dtype, NULL,
                             NULL, NULL, NULL, r_qdat, &nmcmp, r_mxqdat),
                             "Read UnstrQuant Section failed\n" );
             }

             break;

           case UNSTRUCT:
             TIO_Call( TIO_Read_UnstrQuant_Section(fid, qid, csectid,
                           TIO_XFER, quant_dtype,
                           NULL, r_qdat, &nmcmp, NULL),
                           "Read UnstrQuant Section failed\n" );


#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

             if (nmcmp > 0) {
               free(r_mxqdat);
               tmalloc(r_mxqdat,(sizeof(double)* nmcmp), 
                       "Failed to allocate r_mxqdat");

               TIO_Call( TIO_Read_UnstrQuant_Section(fid, qid, csectid,
                             TIO_XFER, quant_dtype,
                             NULL, r_qdat, &nmcmp, r_mxqdat),
                             "Read UnstrQuant Section failed\n" );
             }

             break;

           default:
               TIO_Call( TIO_Read_PointQuant_Section(fid, qid, nsectid,
                             TIO_XFER, quant_dtype, NULL, r_qdat),
                             "Read PointQuant Section failed\n" );
#ifdef DEBUG
             chkdbl("qdat", qdat, r_qdat, n); 
             /* update data for next iteration */
             for (j=0; j<n; j++) qdat[j] += (double) 1000000;
#endif

         }

         TIO_Call( TIO_Close_Quant(fid, qid),      "Close Quant failed\n" );

      }

      t3 = MPI_Wtime();


      /* Close up */


      TIO_Call( TIO_Close_Material(fid, matid), "Close Material failed\n" );


      TIO_Call( TIO_Close_Mesh(fid, msid),      "Close Mesh failed\n" );


      TIO_Call( TIO_Close_State(fid, stid),     "Close State failed\n" );

      mpierr=MPI_Barrier(mpicomm);

      TIO_Call( TIO_Close(fid),                 "Close File failed\n");


      t4 = MPI_Wtime();

      t3=t3-t2;
      t4=t4-t1;

      mpierr = MPI_Reduce(&t3, &tr3, 1, MPI_DOUBLE, MPI_MIN, 0, mpicomm);
      mpierr = MPI_Reduce(&t4, &tr4, 1, MPI_DOUBLE, MPI_MAX, 0, mpicomm);

      if (mpirank == 0) {
        fprintf(stdout, "\n#15  READ Section -> Mat/Quant Max time taken (per proc) : %f\n", tr3);
        fprintf(stdout, "#16  READ Section -> Total     Max time taken (per proc) : %f\n\n", tr4);
      }

    /* Check validity of read data */

      mpierr = MPI_Barrier(mpicomm);

    }  /* rflag == SECTION */

    /* free the scratch space */

    free(r_xc);
    free(r_yc);
    free(r_zc);
    free(r_mdat);
    if (nquant > 0)
      free(r_qdat);
    if (meshtype == UNSTRUCT) {
      free(r_conn);
      free(r_nids);
      free(r_cids);
    }
    if (nmix > 0) {
      free(r_ptr);
      free(r_ncomp);
      free(r_vf);
      free(r_mxmdat);
    if (nquant > 0)
      free(r_mxqdat);
    }
/*    if (meshtype == POINT || meshtype == QUAD) { */
    if (meshtype == QUAD && (rflag == SECTION) ) {
       free(nh);
       free(nl);
    }

  }

  /* free the initial reference array space */

  if (meshtype == QUAD && mpirank == 0 && ( (mode == WRITE) || (mode == APPEND) ) ) {

     free(whole_xc);
     free(whole_yc);
     free(whole_zc);
  }

  free(xc);
  free(yc);
  free(zc);
  free(mdat);
  if (nquant > 0)
    free(qdat);
  if (meshtype == UNSTRUCT) {
    free(conn);
    free(nids);
    free(cids);
  }
  if (meshtype == POINT)
    free(pids);
  if (nvar > 0 && mpirank ==0) {
    free(intvar);
    free(dblvar);
  }
  if (nmix > 0) {
    free(ptr);
    free(ncomp);
    free(vf);
    free(mxmdat);
    if (nquant > 0)
      free(mxqdat);
  }
  /*
   *  Finish
   */

  return (0);
}


void chkint(char *name, int *a, int *r, int n)
{
  int i, ner;

  ner = 0;
  for (i=0; i<n; i++) {
    if (a[i]==r[i]) continue;
    if (!ner) printf("PE %d: checking %s ... err\n",me,name);
    if (++ner<10) printf("PE %d:    %s[%d]=%d  !=  r_%s[%d]=%d\n",me,name,i,a[i],name,i,r[i]);
  }
  if (ner) printf("PE %d:    %s  ner=%d\n",me,name,ner);
  else printf("PE %d: checking %s ... ok\n",me,name);
}

void chkdbl(char *name, double *a, double *r, int n)
{
  int i, ner;

  ner = 0;
  for (i=0; i<n; i++) {
    if (a[i]==r[i]) continue;
    if (!ner) printf("PE %d: checking %s ... err\n",me,name);
    if (++ner<10) printf("PE %d:    %s[%d]=%f  !=  r_%s[%d]=%f\n",me,name,i,a[i],name,i,r[i]);
  }
  if (ner) printf("PE %d:    %s  ner=%d\n",me,name,ner);
  else printf("PE %d: checking %s ... ok\n",me,name);
}

