#-*- Mode : makefile; -*-  emacs mode specifier

#
# Compiler & flag macros for different compilers / systems
#



#
# Stuff that should be common across all compilers/systems:
# (can overload in lower macros if necessary though)
#

TPP_DIR		?=	$(realpath ${PWD}/../../../..)
F2C_DIR		?=	$(realpath ${PWD}/../../../..)/F2C

TPP		=	$(TPP_DIR)/bin/tpp.py

ARFLAGS		=


VALID_COMPILER	=	0


#
#  INTEL  --------------------------------------------------
#

ifeq (intel, $(findstring intel,${HPC_COMPILER}))

VALID_COMPILER	=	1

CC_SER		=	icc -fpic
CC_PAR		=	mpicc -fpic
CC_VERSION	=	icc -V 2>&1

CFLAGS_OPT	=	-O2
CFLAGS_DBG	=	-g -C -traceback

CINC		=	-I
COBJ		=	-c
COUT		=	-o

CPP_FLAGS	=

FC_SER		=	ifort -free -heap-arrays 64 -fpic
FC_PAR		=	mpif90 -free -heap-arrays 64 -fpic
FC_VERSION	=	ifort -V  2>&1
FFLAGS_OPT	=	-O2
FFLAGS_DBG	=	-g -C -traceback

FINC		=	-I
FOBJ		=	-c
FOUT		=	-o

FMODULES	=	*.mod

FPP_FLAGS	=	 -fpp $(FPP_DEF)_TYPHIO_F2003_STDERR
FPP_DEF		=	-D
FPP_UNDEF	=	-U

F2C_DEF		=	F2C_UNDERSCORE

endif

# ----------------------------------------------------------



#
# PORTLAND GROUP (PGI)  ------------------------------------
#

ifeq (pgi, $(findstring pgi,${HPC_COMPILER}))

VALID_COMPILER	=	1

CC_SER		=	pgcc -tp px-64 -c89 -fpic
CC_PAR		=	mpicc -tp px-64 -c89 -fpic
CC_VERSION	=	pgcc -V 2>&1

CFLAGS_OPT	=	-O2
CFLAGS_DBG	=	-g -C -Minform=warn

CINC		=	-I
COBJ		=	-c
COUT		=	-o

CPP_FLAGS	=

FC_SER		=	pgf90 -tp px-64 -Mfree -fpic
FC_PAR		=	mpif90  -tp px-64 -Mfree -fpic
FC_VERSION	=	pgf90 -V  2>&1
FFLAGS_OPT	=	-O2
FFLAGS_DBG	=	-g -C

FINC		=	-I
FOBJ		=	-c
FOUT		=	-o

FMODULES	=	*.mod

FPP_FLAGS	=	-Mpreprocess $(FPP_DEF)_TYPHIO_F2003_STDERR
FPP_DEF		=	-D
FPP_UNDEF	=	-U

F2C_DEF		=	F2C_UNDERSCORE

endif

# ----------------------------------------------------------



#
# IBM  -----------------------------------------------------
#

ifeq (xl, $(findstring xl,${HPC_COMPILER}))

VALID_COMPILER	=	1

CC_SER		=	xlc -qlanglvl=stdc99
CC_PAR		=	mpcc -qlanglvl=stdc99
CC_VERSION	=	xlc -qversion

CFLAGS_OPT	=	-O2
CFLAGS_DBG	=	-g -C -qfullpath

CINC		=	-I
COBJ		=	-c
COUT		=	-o

CPP_FLAGS	=

FC_SER		=	xlf90 -qfree=F90
FC_PAR		=	mpxlf90 -WF-qfpp -qfree=F90
FC_VERSION	=	xlf90 -qversion
FFLAGS_OPT	=	-O2
FFLAGS_DBG	=	-g -C -qfullpath

FINC		=	-I
FOBJ		=	-c
FOUT		=	-o

FMODULES	=	*.mod

FPP_FLAGS	=	 -WF,-qfpp $(FPP_DEF)_TYPHIO_F2003_STDERR
FPP_DEF		=	-WF,-D
FPP_UNDEF	=	-WF,-U

F2C_DEF		=	F2C_NOCHANGE

endif

# ----------------------------------------------------------



#
# SUN  -----------------------------------------------------
#

ifeq (sun, $(findstring sun,${HPC_COMPILER}))

VALID_COMPILER	=	1

CC_SER		=	suncc -xtarget=generic -Kpic
CC_PAR		=	mpicc -xtarget=generic -Kpic
CC_VERSION	=	suncc -V 2>&1

CFLAGS_OPT	=	-O2
CFLAGS_DBG	=	-g -xcheck

CINC		=	-I
COBJ		=	-c
COUT		=	-o

CPP_FLAGS	=

FC_SER		=	sunf90 -xtarget=generic -free -Kpic
FC_PAR		=	mpif90 -xtarget=generic -free -Kpic
FC_VERSION	=	sunf90 -V  2>&1
FFLAGS_OPT	=	-O2
FFLAGS_DBG	=	-g -xcheck=all

FINC		=	-I
FOBJ		=	-c
FOUT		=	-o

FMODULES	=	*.mod

FPP_FLAGS	=	-fpp $(FPP_DEF)_TYPHIO_F2003_STDERR
FPP_DEF		=	-D
FPP_UNDEF	=	-U

F2C_DEF		=	F2C_UNDERSCORE

endif

# ----------------------------------------------------------



#
# GNU  -----------------------------------------------------
#

ifeq (gcc, $(findstring gcc,${HPC_COMPILER}))

VALID_COMPILER	=	1

CC_SER		=	gcc -fpic -ansi
CC_PAR		= 	mpicc -fpic -ansi
CC_VERSION	=	mpicc --version 2>&1

CFLAGS_OPT	=	-O2
CFLAGS_DBG	=	-g -ansi -pedantic -Wall -Wextra -Wno-long-long -Wno-unused-parameter

CINC		=	-I
COBJ		=	-c
COUT		=	-o

CPP_FLAGS	=

FC_SER		=	gfortran -fpic 
FC_PAR		= 	mpif90 -fpic 
FC_VERSION	=	mpif90 --version 2>&1

FFLAGS_OPT	=   -O2
FFLAGS_DBG	=  -g  -pedantic -Wall -Wextra -Wno-long-long -Wno-unused-parameter

FINC		= -I
FOBJ		= -c
FOUT		= -o

FMODULES	= *.mod

FPP_FLAGS	= -cpp $(FPP_DEF)_TYPHIO_F2003_STDERR
FPP_DEF		= -D
FPP_UNDEF	= -U

F2C_DEF		= F2C_UNDERSCORE


endif


# ----------------------------------------------------------


#
# EOF
#
