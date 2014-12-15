
FILE = <namehere>.F90

C_FLAGS   = -O0 -g
F90_FLAGS = -O0 -g

TIO_DIR = /prod/$(HPC_SYSTEM)/$(HPC_ARCH)/$(HPC_COMPILER)/$(HPC_MPI)/codes/typhonio/1.5
TIO_LIB = -L$(TIO_DIR)/lib -ltyphonio_f -ltyphonio
TIO_INC = -I$(TIO_DIR)/include

ZDIR		=	/lib64

HDF5_DIR = /prod/$(HPC_SYSTEM)/$(HPC_ARCH)/$(HPC_COMPILER)/$(HPC_MPI)/external/hdf5/1.8.5
HDF5_LIB = -L$(HDF5_DIR)/lib -lhdf5 -L$(ZDIR)/lib -lz  

INCLUDES = $(TIO_INC)
LIBS = $(TIO_LIB) $(HDF5_LIB)  -lifcore

TEST_FILES := $(wildcard tiof_*.test)
#TEST_FILES := tiof_variable.test

all: $(TEST_FILES:.test=.x)
	for FILE in $(TEST_FILES:.test=.x); \
	  do \
	    ./$$FILE; \
	  done;

file: $(FILE:.F90=.x)

%.x: tunit.o %.o %_suite.o
	icc  $(C_FLAGS) -o $@ $^ $(LIBS)

%.x: %.o
	ifort $(F90_FLAGS) -o $@ $^ $(TIO_LIB) $(HDF5_LIB) 

%_suite.o: %.f90
	icc -c -o $@ $(C_FLAGS) $*_suite.c

%.f90: %.test
	echo "./tunit.py $< $@ $*_suite.c"
	./tunit.py $< $@ $*_suite.c

%.o: %.f90
	ifort -c -o $@ $(F90_FLAGS) $(INCLUDES) $<

%.o: %.F90
	ifort -c -o $@ $(F90_FLAGS) $(INCLUDES) $<

%.o: %.c
	icc -c -o $@ $(C_FLAGS) $<

clean:
	rm -f *.o *.mod *~ *.x $(TEST_FILES:.test=.f90) *_suite.c *.h5
