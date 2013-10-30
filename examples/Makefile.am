bin_PROGRAMS = nbcrc nbcrc_mpi nbcrc_mpi_nbcr nbcrs himeno himeno_netbound himeno_nbcr himeno_netbound_nbcr
#bin_PROGRAMS = nbcrs himeno himeno_nbcr

RDMATLS_DIR=/home/usr2/11D37048/usr/tools/libibtls

INCLUDE_DIR=${RDMATLS_DIR}/include 
LIBRARY_DIR=${RDMATLS_DIR}/lib
SRC_DIR    =../src
SRC_TARGET = $(SRC_DIR)/nbcr.c $(SRC_DIR)/nbcr_util.c
CC = mpicc

nbcrc_SOURCES = nbcrc.c ${SRC_TARGET}
nbcrc_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
nbcrc_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

nbcrc_mpi_SOURCES = nbcrc_mpi.c ${SRC_TARGET}
nbcrc_mpi_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
nbcrc_mpi_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

nbcrc_mpi_nbcr_SOURCES = nbcrc_mpi_nbcr.c ${SRC_TARGET}
nbcrc_mpi_nbcr_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
nbcrc_mpi_nbcr_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

nbcrs_SOURCES = nbcrs.c ${SRC_TARGET}
nbcrs_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
nbcrs_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

himeno_SOURCES = himenoBMTxps.c ${SRC_TARGET}
himeno_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
himeno_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

himeno_netbound_SOURCES = himenoBMTxps_netbound.c ${SRC_TARGET}
himeno_netbound_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
himeno_netbound_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

himeno_nbcr_SOURCES = himenoBMTxps_nbcr.c ${SRC_TARGET}
himeno_nbcr_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
himeno_nbcr_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

himeno_netbound_nbcr_SOURCES = himenoBMTxps_netbound_nbcr.c ${SRC_TARGET}
himeno_netbound_nbcr_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR} -I../include
himeno_netbound_nbcr_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm