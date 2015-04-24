
CMP		=	mpicc $(CPREPRO)
CPREPRO 	=	-D_LARGE_FILES
DFLAGS		=	-g -DDEBUG
CFLAGS		=	-O3 -DPARALLEL -DHINTS

LNK		=	mpicc $(CFLAGS) -DPARALLEL -shared-intel

TIO_DIR   	= 	<path>/<to>/<typhonio>
TIOINC   	= 	-I$(TIO_DIR)/include
TIOLIBS		=	-L$(TIO_DIR)/lib -ltyphonio

HDF5DIR		=	<path>/<to>/<hdf5>/1.8.5
HDF5INC		=	-I$(HDF5DIR)/include
HDF5LIB		=	-L$(HDF5DIR)/lib -lhdf5
ZLIB		= 	<path>/<to>/<zlib> -lz

INCS		=	$(TIOINC) $(HDF5INC)
LIBS		=	$(TIOLIBS) $(HDF5LIB) $(ZLIB)


default     :
	$(CMP) $(CFLAGS) $(INCS) $(OPTS) iobench.c -c -o iobench.o
	$(LNK) $(CFLAGS) iobench.o $(LIBS) -o iobench.x

debug     :
	$(CMP) $(DFLAGS) $(INCS) $(OPTS) iobench.c -c -o iobench.o
	$(LNK) $(DFLAGS) iobench.o $(LIBS) -o iobench.x

clean	:
	- rm *.o *.x

