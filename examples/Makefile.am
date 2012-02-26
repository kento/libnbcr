bin_PROGRAMS = nbcrc nbcrs

RDMATLS_DIR=/home/usr2/11D37048/usr/tools/libibtls

INCLUDE_DIR=${RDMATLS_DIR}/include
LIBRARY_DIR=${RDMATLS_DIR}/lib
SRC_DIR    =../src
SRC_TARGET = 


nbcrc_SOURCES = nbcrc.c ${SRC_TARGET}
nbcrc_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR}
nbcrc_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm

nbcrs_SOURCES = nbcrs.c ${SRC_TARGET}
nbcrs_CFLAGS  = -I${INCLUDE_DIR} -I${SRC_DIR}
nbcrs_LDADD  = -L${LIBRARY_DIR} -libtls -lpthread -lrdmacm