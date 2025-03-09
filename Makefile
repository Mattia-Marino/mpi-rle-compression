# Compiler definitions
CC = gcc
MPICC = mpicc

# Compiler flags
CFLAGS = -O3 -g -std=c99

# Target executables
TARGETS = parallel_compress parallel_decompress serial_compress serial_decompress

# Subdirectory definitions
COMP_SRC_DIR = ./compression/src/
DECP_SRC_DIR = ./decompression/src/

# Default target
all : ${TARGETS}

# Parallel compression target
parallel_compress : \
    ${COMP_SRC_DIR}parallel/main.o \
    $(COMP_SRC_DIR)compressor.o \
    $(COMP_SRC_DIR)common.o \
    $(COMP_SRC_DIR)buffIter.o \
    $(COMP_SRC_DIR)writeBuff.o \
    $(COMP_SRC_DIR)u64array.o
	${MPICC} ${CFLAGS} -o parallel_compress $^ -lm

# Parallel decompression target
parallel_decompress: \
    $(DECP_SRC_DIR)parallel/main.o \
    $(DECP_SRC_DIR)mpi_common.o \
    $(DECP_SRC_DIR)mpi_decompressor.o
	$(MPICC) ${CFLAGS} -o parallel_decompress $^ -lm

# Serial compression target
serial_compress : \
    $(COMP_SRC_DIR)serial/main.o \
    $(COMP_SRC_DIR)common.o \
    $(COMP_SRC_DIR)buffIter.o \
    $(COMP_SRC_DIR)writeBuff.o \
    $(COMP_SRC_DIR)u64array.o
	${CC} ${CFLAGS} -o serial_compress $^ -lm

# Serial decompression target
serial_decompress: \
    $(DECP_SRC_DIR)serial/main.o \
    $(DECP_SRC_DIR)common.o \
    $(DECP_SRC_DIR)decompressor.c
	$(CC) $(CFLAGS) -o serial_decompress $^ -lm


# Object file rules for compression
$(COMP_SRC_DIR)%.o : $(COMP_SRC_DIR)%.c
	${CC} ${CFLAGS} -o $@ -c $<

$(COMP_SRC_DIR)serial/main.o: $(COMP_SRC_DIR)serial/main.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(COMP_SRC_DIR)parallel/main.o: $(COMP_SRC_DIR)parallel/main.c
	$(MPICC) $(CFLAGS) -o $@ -c $<


# Object file rules for decompression
$(DECP_SRC_DIR)%.o : $(DECP_SRC_DIR)%.c
	${CC} ${CFLAGS} -o $@ -c $<

$(DECP_SRC_DIR)serial/main.o: $(DECP_SRC_DIR)serial/main.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(DECP_SRC_DIR)parallel/main.o: $(DECP_SRC_DIR)parallel/main.c
	$(MPICC) $(CFLAGS) -o $@ -c $<


# Clean target
clean :
	rm -f ${TARGETS}
	find . -name "*.o" -type f -delete
	rm -f *~
	make clearDM

# Clear data and metadata files
clearDM :
	rm -f *.data
	rm -f *.meta