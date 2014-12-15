usage		:
			@ echo
			@ echo "Build :"
			@ echo "   make typhonio_parallel"
			@ echo

typhonio_parallel :       
			${MAKE} -f Makefile.typhonio_ptest all


all :			tio_test

INCDIR		=	$(CFLAGS) -I$(TIODIR)/include 
LIBDIR		=	$(TIOLIBS) $(HDF5LIBSDBG)

tio_test     :		tio_writequad.x \
                	tio_readquad.x \
					tio_point.x \
					tio_unstr.x


.SUFFIXES       :
.SUFFIXES       :	.c .o .x

.c.o            :
			@echo "Making $@"
			$(CMP) -c $(INCDIR) $(MODINC).  $< -o $(@:.x=.o)
			@echo
.c.x            :
			@echo "Making $@"
			$(CMP) -c $(INCDIR) $(MODINC). $< -o $(@:.x=.o)
			$(LNK) $(@:.x=.o) $(LIBDIR)  -o $@
			@echo



#copy    : test
#		@echo "Copy $@"
#	  - ./tio_copy.x  


clean		:
		- rm *.o *.x *.h5

allclean        : 
		- rm *.o *.out *.x *.h5 *.log


qa  :
		- make typhonio_parallel
		  make clean
		  
