// SPDX-License-Identifier: GPL-3.0

#include <stdio.h>
#include <sys/stat.h>

#include "../include/compressor.h"

void initMetaFile(FILE *metaFile, unsigned int lengthOfRunInBits,
		  unsigned long numRuns, unsigned int numDprocs)
{
	// Write first 8 bits as number of processes to use
	// during the decompression.
	fputc(numDprocs, metaFile);

	// Write first 48 bits as total number of runs
	for (int i = 1; i <= 6; ++i) {
		unsigned char chunk = (numRuns >> (48 - i * 8)) & 0xFF;

		fputc(chunk, metaFile);
	}

	// Write next 8 bits as the length of a run in bits
	fputc((int)lengthOfRunInBits, metaFile);
}

void initDataFile(FILE *dataFile, unsigned int keySize)
{
	// Write first 8 bits as keySize
	fputc(keySize, dataFile);
}

unsigned long getFileSize(char *filename)
{
	struct stat buff;

	if (stat(filename, &buff) != 0) {
		perror("stat"); // Print error message if stat fails
		return 0; // Or handle the error appropriately
	}

	return buff.st_size;
}
