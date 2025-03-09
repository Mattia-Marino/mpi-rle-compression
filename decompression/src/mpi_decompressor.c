// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/mpi_common.h"
#include "../include/mpi_decompressor.h"

void getMetaData(FILE *meta, FILE *data, unsigned char *mUsed,
		 unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		 unsigned char *runLen, unsigned char *keyLen,
		 uint64_t *numRuns, unsigned char *expProcs)
{
	// gets data out of the beginning of the meta, data files on the bit
	// lengths of keys and the number of runs
	*mUsed = *dUsed = 0;
	*mCur = fgetc(meta);
	*dCur = fgetc(data);

	unsigned char expProcsSize = 8; // should be 1 bit, but time
	unsigned char tagSize = 8; // should be 6 bits, but time
	unsigned char numRunSize = 48;

	*expProcs = get(meta, mUsed, mCur, expProcsSize);
	*numRuns = get(meta, mUsed, mCur, numRunSize);
	*runLen = (char)get(meta, mUsed, mCur, tagSize);
	*keyLen = (char)get(data, dUsed, dCur, tagSize);
}

void decompress(FILE *meta, FILE *data, char *outBuf, unsigned char *mUsed,
		unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		unsigned char runLen, unsigned char keyLen, uint64_t numRuns)
{
	// iterates throuh meta to find a run length, then through data for that
	// length. continues for numRuns iterations through meta.
	uint64_t run, key, i, j, k, bufIdx;
	unsigned char oCur;
	unsigned char oUsed = 0;

	bufIdx = 0;
	for (k = 0; k < numRuns; ++k) {
		run = get(meta, mUsed, mCur, runLen);
		// escape code indicating a series of unique keys
		if (run == 0) {
			run = get(meta, mUsed, mCur, runLen);
			for (j = 0; j < run; ++j) {
				// iterate through unique keys, writing them to the file
				key = get(data, dUsed, dCur, keyLen);
				put(outBuf, key, &oUsed, &oCur, keyLen,
				    &bufIdx);
			}
		}
		// "proper" run (repetition of the same key)
		else {
			key = get(data, dUsed, dCur, keyLen);
			for (j = 0; j < run; ++j) {
				// write the key as many times as the meta file says to
				put(outBuf, key, &oUsed, &oCur, keyLen,
				    &bufIdx);
			}
		}
	}
}
