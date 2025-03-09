/* SPDX-License-Identifier: GPL-3.0 */

/*
 * Header file for compression utility functions.
 *
 * This header defines functions and constants used for compressing data
 * and writing metadata. It provides functions for initializing meta and
 * data files, and for getting the size of a file.
 */

// #include <fcntl.h>
// #include <inttypes.h>
// #include <mpi.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <sys/types.h>

// #include "buffIter.h"
// #include "common.h"
// #include "writeBuff.h"

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <stdio.h>

// static const char data[5] = ".data";
// static const char meta[5] = ".meta";

// // Used to convert integers to strings
// char num[20];

/**
 * @brief Initializes the metadata file with process, run count, and run length info.
 *
 * @param metaFile Pointer to the metadata file.
 * @param lengthOfRunInBits Length of a run in bits.
 * @param numRuns Total number of runs.
 * @param numDprocs Number of decompression processes.
 */
void initMetaFile(FILE *metaFile, unsigned int lengthOfRunInBits,
		  unsigned long numRuns, unsigned int numDprocs);

/**
 * @brief Initializes the data file with the key size.
 *
 * @param dataFile Pointer to the data file.
 * @param keySize Size of the key.
 */
void initDataFile(FILE *dataFile, unsigned int keySize);

/**
 * @brief Gets the size of a file.
 *
 * @param filename: Name of the file.
 * @return Size of the file in bytes.
 */
unsigned long getFileSize(char *filename);

// "You are on this council but we do not grant you the rank of master."
#define MASTER_RANK 0

#endif
