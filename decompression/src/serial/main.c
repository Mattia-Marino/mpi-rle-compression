// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "../../include/common.h"
#include "../../include/decompressor.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("usage: ./decompress [input name] [output name]\n");
		return -1;
	}

	struct timeval tvStart, tvEnd;

	gettimeofday(&tvStart, 0);

	// get input files

	/* char* dataName = malloc(sizeof(argv[1]) + 5); */
	/* char* metaName = malloc(sizeof(argv[1]) + 5); */
	/* printf("sizeof %s: %i\n", argv[1], strlen(argv[1])); */
	/* memmove(dataName, argv[1], sizeof(argv[1])); */
	/* memmove(metaName, argv[1], sizeof(argv[1])); */
	char *dataName = malloc(strlen(argv[1]) + 5);
	char *metaName = malloc(strlen(argv[1]) + 5);

	memmove(dataName, argv[1], strlen(argv[1]));
	memmove(metaName, argv[1], strlen(argv[1]));

	strcat(dataName, ".data");
	strcat(metaName, ".meta");

	FILE *data = fopen(dataName, "rb");
	FILE *meta = fopen(metaName, "rb");
	FILE *out = fopen(argv[2], "wb");

	// variables for tracking relevant features of files
	uint64_t numRuns;
	unsigned char mUsed, dUsed, mCur, dCur, runLen, keyLen;

	// actually do work
	getMetaData(meta, data, &mUsed, &dUsed, &mCur, &dCur, &runLen, &keyLen,
		    &numRuns);
	printf("numRuns: %" PRIu64 " | runLen: %u | keyLen: %u\n", numRuns,
	       runLen, keyLen);
	decompress(meta, data, out, &mUsed, &dUsed, &mCur, &dCur, runLen,
		   keyLen, numRuns);

	struct timeval elapsedTime;

	gettimeofday(&tvEnd, 0);
	subtractTime(&tvStart, &tvEnd, &elapsedTime);
	printf("Elapsed time: %ld.%ld06\n", elapsedTime.tv_sec,
	       elapsedTime.tv_usec);

	// tidy up
	fclose(data);
	fclose(meta);
	fclose(out);
	return 0;
}
