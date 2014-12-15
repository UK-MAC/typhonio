
FILES := $(wildcard *.F90)

C_FLAGS   = -O0 -g
F90_FLAGS = -O0 -g


TIO_DIR = 
TIO_LIB = -L$(TIO_DIR)/lib -ltyphonio_f -ltyphonio
TIO_INC = -I$(TIO_DIR)/include

HDF5_DIR = 

HDF5_LIB = -L$(HDF5_DIR)/lib -lhdf5

INCLUDES = $(TIO_INC)
LIBS = $(TIO_LIB) $(HDF5_LIB) -lz -lifcore

all: $(FILES:.F90=.x)
	for FILE in $(FILES:.F90=.x); \
	  do \
	    echo $$FILE; \
	    ./$$FILE; \
	  done;

%.x: %.F90
	ifort -o $@ $(F90_FLAGS) $(INCLUDES) $< $(TIO_LIB) $(HDF5_LIB)

clean:
	rm -f *.o *.mod *~ *.x
