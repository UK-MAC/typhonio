#-*- Mode : makefile; -*-  emacs mode specifier

##############################################################
#
#   Makefile for TyphonIO library
#
##############################################################


##############################################################
#
# The basics:
#

PACKAGE			:=	TyphonIO
NAME			:=	typhonio
VERSION			:=	1.6

UNIXGROUP		:=	typhon

MAKE			?=	gmake -s
SMAKE			?=	$(MAKE) -s


BUILD			=	${HPC_COMPILER}${EXTENSION}


# Version in Major, Minor, Revision format (M.N.R)
VERSION_MNR		=	` echo $(VERSION) | 		 			\
			 	  awk 'BEGIN {FS="."}; 					\
					{if     (NF==1) print ($$1 ".0.0"); 		\
					else if (NF==2) print ($$1 "." $$2 ".0"); 	\
					else            print ($$1 "." $$2 "." $$3); }' `



##############################################################
#
# Directories:
#

SRC_DIR			=	$(realpath ${PWD})

BUILD_DIR		?=	$(realpath ${PWD}/..)/build
SPEC_BUILD_DIR		?=	$(BUILD_DIR)/$(BUILD)

PROD_DIR		?=	/prod/${HPC_COMPILER}



##############################################################
#
# HDF5
#

HDF5_VERSION		?=	1.8.5
HDF5_DIR		?=	${PROD_DIR}/external/hdf5/${HDF5_VERSION}




##############################################################
#
#
#

EXTENSION_OPT		=	
EXTENSION_DBG		=	-debug
EXTENSION		=	$(EXTENSION_$(OPT_OR_DBG))



##############################################################
#
#
#

default	:	usage
		@ echo "--------------------------------------------------"
		@ echo ""
		@ echo "Current build environment: "
		@ echo ""
		@ ${SMAKE} echosys
		@ echo "--------------------------------------------------"
		@ echo ""

usage	:
		@ echo ""
		@ echo "Usage:"
		@ echo ""
		@ echo "   make config"
		@ echo "   make install"
		@ echo ""
		@ echo "   make clean"
		@ echo "   make cleanall		      - clean all builds"
		@ echo ""
		@ echo "   make [BUILD_DIR=xxx] preinstall    - moves src to BUILD_DIR"
		@ echo "   make [BUILD_DIR=xxx] locinstall    - instigates build"
		@ echo ""
		@ echo "Shortcuts:"
		@ echo ""
		@ echo "   make debug                         - make install_debug"
		@ echo "   make tar        		      - creates tar file in ../"
		@ echo ""
		@ echo "Environment variables:"
		@ echo ""
		@ echo "   HPC_COMPILER"
		@ echo ""
		@ echo "   VERSION                            - TyphonIO version"
		@ echo "   BUILD_DIR                          - path of build directory"
		@ echo "   HDF5_DIR                           - path of HDF5 directory"
		@ echo "   HDF5_VERSION                       - HDF5 version"
		@ echo ""
		@ echo "   CPREPRO_EXTRA                      - extra settings for C preprocessor"
		@ echo "   FPREPRO_EXTRA                      - extra settings for Fortran preprocessor"
		@ echo ""
		@ echo "   NOF90=1                            - switches off build of F90 library"
		@ echo "   USEMPI=1                           - switches on build with MPI library"
		@ echo ""

help	:	usage


##############################################################
#
#
#


ifdef INCLUDEC
include Makefile.Ccommon
endif
ifdef INCLUDEF90
include Makefile.F90common
endif


ifdef ${DEBUG}
OPT_OR_DBG	?=	DBG
else
OPT_OR_DBG	?=	OPT
endif


USEMPI          ?=      0
NOEXPORT	?=	0
NOF90		?=	0
NOSTATIC	?= 	0
NOSHARED	?=	1

ifeq (${USEMPI}, 1)
SER_OR_PAR	?=	PAR
else
SER_OR_PAR	?=	SER
endif



%_debug	:
		@ ${MAKE} OPT_OR_DBG=DBG $(subst _debug,,$@)
%_opt	:
		@ ${MAKE} OPT_OR_DBG=OPT $(subst _opt,,$@)

debug	:
		@ ${MAKE} install_debug




##############################################################
#
#
#

libcheck	:	
			@ echo "  ---  Running HDF5 libcheck..."
			@- rm libcheck.x
			$(CC) $(CINC)$(HDF5_DIR)/include libcheck.c -L$(HDF5_DIR)/lib -lhdf5 -lz -o libcheck.x
			@ ./libcheck.x; if [ $$? -gt 0 ];then			\
				echo "          ...FAILED";			\
				echo "          Throwing error to abort";	\
				rm libcheck.x;					\
			  else							\
				echo "          ...successful";			\
			  fi
			@ rm libcheck.x		# Fails and abort make is already rm-ed above
			@- rm libcheck.o libcheck.lst


##############################################################
#
#
#

ifdef ERROR
$(error $(ERROR))
endif

envcheck	:
			@ if [ ! $$HPC_COMPILER ]; then				\
				${SMAKE} ERROR="HPC_COMPILER unset";		\
			fi
			@ if [ $(VALID_COMPILER) = 0 ]; then			\
				${SMAKE} ERROR="HPC_COMPILER not known";	\
			fi

##############################################################
#
#
#

prebuild	:	makedirs copysrc
			@ echo "  ---  Done prebuild"

makedirs	:
			@- mkdir -p $(SPEC_BUILD_DIR)/include $(SPEC_BUILD_DIR)/lib $(SPEC_BUILD_DIR)/src
			@- if [ ${NOF90} = 0 ]; then 							\
				mkdir -p $(SPEC_BUILD_DIR)/src/F90;					\
			   fi
copysrc		:
			@- cp -p $(SRC_DIR)/*.c $(SRC_DIR)/*.h $(SPEC_BUILD_DIR)/src/
			@- cp -p Makefile* $(SPEC_BUILD_DIR)/src/
			@- if [ ${NOF90} = 0 ]; then 							\
			 	cp -p	$(SRC_DIR)/F90/*.c	$(SRC_DIR)/F90/*.h 			\
					$(SRC_DIR)/F90/*.F90	$(SRC_DIR)/F90/*.tpp			\
					$(SRC_DIR)/F90/*.fpp 	$(SRC_DIR)/F90/*.sh			\
				$(SPEC_BUILD_DIR)/src/F90/;						\
				cp -p Makefile* $(SPEC_BUILD_DIR)/src/F90/;				\
			   fi

makelibs	:
			@ if [ ${NOSTATIC} = 0 ]; then 						\
				cd $(SPEC_BUILD_DIR)/src;						\
				${MAKE} INCLUDEC=1 SPEC_BUILD_DIR=$(SPEC_BUILD_DIR) lib includefiles;	\
			else										\
				echo "  ---  No C library";						\
			fi
			 if [ ${NOF90} = 0 ]; then 							\
				cd $(SPEC_BUILD_DIR)/src/F90;						\
				${MAKE} INCLUDEF90=1 SPEC_BUILD_DIR=$(SPEC_BUILD_DIR) lib includefiles;	\
			else										\
				echo "  ---  No F90 library";						\
			fi
			@ if [ ${NOSHARED} = 0 ]; then 						\
				cd $(SPEC_BUILD_DIR)/src;						\
				${MAKE} INCLUDEC=1 SPEC_BUILD_DIR=$(SPEC_BUILD_DIR) LIBNAME=lib$(NAME).so sharedlib includefiles;	\
			else										\
				echo "  ---  No C shared library";					\
			fi

lib		:	$(OBJ)
			@- rm $(SPEC_BUILD_DIR)/lib/$(LIBNAME)
			@ $(AR) $(ARFLAGS) rv $(SPEC_BUILD_DIR)/lib/$(LIBNAME) $?
			@ echo "  ---  Built $(TITLE)";

sharedlib	:	$(OBJ)
			@- rm $(SPEC_BUILD_DIR)/lib/$(LIBNAME)
			$(CC) -shared -o $(SPEC_BUILD_DIR)/lib/$(LIBNAME) $?
			@ echo "  ---  Built $(TITLE) - shared";

includefiles		:	
			@ cp -f $(INCLUDEFILES) $(SPEC_BUILD_DIR)/include/
			@ echo "  ---  Copied include files";


preexport	:
			@- mkdir -p $(EXPORT_DIR)/include
			@- mkdir -p $(EXPORT_DIR)/lib
			@- chgrp -R $(UNIXGROUP) $(EXPORT_DIR)
			@- chmod -R 755 $(EXPORT_DIR)
			@- chmod -R g+s $(EXPORT_DIR)
			@- rm -f $(EXPORT_DIR)/include/* $(EXPORT_DIR)/lib/*


install		: 	envcheck prebuild configlog makelibs

#
# Provide two separate preinstall and locinstall steps
#   - preinstall can then move src into local space on a system
#   - locinstall can then build in that local space whilst, say, on a compute node
#
preinstall	:	envcheck prebuild
			@ echo "  ---  preinstall sent to:"
			@ echo "       $(SPEC_BUILD_DIR)"

locinstall	:	envcheck configlog makelibs

locclean	:
			rm $(SPEC_BUILD_DIR)/src/*.o 		\
				$(SPEC_BUILD_DIR)/include/*	\
				$(SPEC_BUILD_DIR)/lib/*


#
# Tar
#

TARFILE		=	typhonIO.tar
TARLIST		=	src/*.c src/*.h src/Makefile*					\
			src/F90/*.tpp src/F90/*.f90 src/F90/*.F90 src/F90/*.c src/F90/*.h 		\
			CHECKS								\
			$(F2C_DIR) $(TPP_DIR)
tar		:
			cd ..; 								\
			tar cvf $(TARFILE) --owner=nobody --group=nobody $(TARLIST);	\
			gzip $(TARFILE)




##############################################################
#
# Config
#
# To get all config options, need to include C and F90 specific makefiles.
# Can't do this for everything, as OBJ, INCLUDEFILES will conflict between C & F90 makefiles.
# (Having them defined differently in those makefiles enable common rule re-use for
#  building libs, copying include files, etc.)
#


echosys		:
			@ if [ ${HPC_COMPILER} ]; then				\
				echo "HPC_COMPILER    = " ${HPC_COMPILER};	\
			  else							\
				echo "HPC_COMPILER    =  unset";		\
			  fi


config		:	envcheck
			@ ${SMAKE} configout

configC		:
			@ echo "# -- Compiler options - C library:"
			@ echo ""
			@ echo "CC              = " $(CC)
			@ echo "CC_VERSION      = " `$(CC_VERSION)`
			@ echo "CFLAGS          = " $(CFLAGS)
			@ echo "CPREPRO         = " $(CPREPRO)

configF90	:
			@ echo "# -- Compiler options - F90 library:"
			@ echo ""
			@ echo "CC              = " $(CC)
			@ echo "CC_VERSION      = " `$(CC_VERSION)`
			@ echo "CFLAGS          = " $(CFLAGS)
			@ echo "CPREPRO         = " $(CPREPRO)
			@ echo ""
			@ echo "FC              = " $(FC)
			@ echo "FC_VERSION      = " `$(FC_VERSION)`
			@ echo "FFLAGS          = " $(FFLAGS)
			@ echo "FPREPRO         = " $(FPREPRO)
			@ echo ""
			@ echo "F2C_DIR         = " $(F2C_DIR)
			@ echo "F2C_DEF         = " $(F2C_DEF)
			@ echo "TPP             = " $(TPP)

configout		:
			@ echo ""
			@ echo "[typhonio]"
			@ echo ""
			@ echo "# -- $(PACKAGE) config for:" $(BUILD)
			@ echo "# ----------------------------------------------------------"
			@ echo "VERSION         = " $(VERSION)
			@ echo "VERSION_MNR     = " $(VERSION_MNR)
			@ echo ""
			@ echo "SVN_URL         = " $(SVN_URL)
			@ echo "SVN_REVISION    = " $(SVN_REVISION)
			@ echo ""
			@ echo "DATE            = " `date`
			@ echo ""
			@ echo ""
			@ echo "# -- HPC Common Environment:"
			@ echo ""
			@ ${SMAKE} echosys
			@ echo ""
			@ echo "# -- Paths:"
			@ echo ""
			@ echo "HDF5_DIR        = " $(HDF5_DIR)
			@ echo ""
			@ echo "SRC_DIR         = " $(SRC_DIR)
			@ echo "BUILD_DIR       = " $(BUILD_DIR)
			@ echo "SPEC_BUILD_DIR  = " $(SPEC_BUILD_DIR)
			@ echo ""
			@ ${SMAKE} INCLUDEC=1 configC
			@ echo ""
			@ ${SMAKE} INCLUDEF90=1 configF90
			@ echo ""
			@ echo ""

CONFIGFILE	=	lib$(NAME).config

configlog	:	makedirs
			@ ${SMAKE} configout > $(SPEC_BUILD_DIR)/lib/$(CONFIGFILE)
			@ echo "  ---  Produced config logfile"



##############################################################
#
# Clean ups
# Can blow away current build, or all builds
#

clean		:
			@ echo "Removing build dir: " $(SPEC_BUILD_DIR)
			@- rm -r $(SPEC_BUILD_DIR)

cleanall	:
			@ echo "Removing all build dirs from: " $(BUILD_DIR)
			@- rm -f -r $(BUILD_DIR)/*



##############################################################
#
# Common compiler gubbins
#

include Makefile.inc

#
# Set up the optimise/debug and serial/parallel variants ---
#

CC			=	$(CC_$(SER_OR_PAR))
FC			=	$(FC_$(SER_OR_PAR))

CPREPRO			+=	$(CPREPRO_$(OPT_OR_DBG)) $(CPREPRO_$(SER_OR_PAR)) $(CPREPRO_EXTRA)
FPREPRO			+=	$(FPREPRO_$(OPT_OR_DBG)) $(FPREPRO_$(SER_OR_PAR)) $(FPREPRO_EXTRA)

CFLAGS			+=	$(CFLAGS_$(OPT_OR_DBG))
FFLAGS			+=	$(FFLAGS_$(OPT_OR_DBG))

# CCMP		= set in C/F90 common makefiles
# FCMP		= set in F90 common makefile

.SUFFIXES		:
.SUFFIXES		:	.inc .tpp .F90 .c .o
.tpp.F90        	:
				$(TPP) $< $@
.F90.o			:
				$(FCMP) ${FOPTIONS} $(FOBJ) $(FOUT) $@ $<
.c.o			:
				$(CCMP) ${COPTIONS} $(COBJ) $(COUT) $@ $<





##############################################################
#
# Some checks...
#

define checkvar
check_$(1)	:	
ifeq ($(origin $(1)), undefined)
	@ echo WARNING: $(1) undefined!
endif
ifeq ($(origin $(1)), default)
	@ echo WARNING: $(1) set to gmake default!
endif
ifeq ($(strip $($(1))),)
	@ echo WARNING: $(1) unset!
endif
endef

define printvar
print_$(1)	:	
			@ echo -e "$(1) \t = $($(1)) \t  $(origin $(1))"
endef


checklist	=	CC CC_VERSION CFLAGS CPREPRO CINC COBJ COUT		\
			FC FC_VERSION FFLAGS FPREPRO FINC FOBJ FOUT FMODULES	\
			FPP_DEF							\
			F2C_DEF

$(foreach var,$(checklist),$(eval $(call printvar,$(var))))
$(foreach var,$(checklist),$(eval $(call checkvar,$(var))))

# check that all necessary macros listed above have been set
check		:
			@ echo "checking compiler specific variables are set..."
			@ ${SMAKE} INCLUDEC=1   $(foreach var,$(checklist),check_$(var))
			@ ${SMAKE} INCLUDEF90=1 $(foreach var,$(checklist),check_$(var))
			@ echo "done"

# print values of all necessary macros
print		:
			@ echo "Variables are set..."
			@ ${SMAKE} INCLUDEC=1   $(foreach var,$(checklist),print_$(var))
			@ ${SMAKE} INCLUDEF90=1 $(foreach var,$(checklist),print_$(var))
			@ echo "done"


#
# EOF
#
