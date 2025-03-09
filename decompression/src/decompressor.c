// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/common.h"
#include "../include/decompressor.h"

void getMetaData(FILE *meta, FILE *data, unsigned char *mUsed,
		 unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		 unsigned char *runLen, unsigned char *keyLen,
		 uint64_t *numRuns)
{
	// gets data out of the beginning of the meta, data files on the bit
	// lengths of keys and the number of runs
	*mUsed = *dUsed = 0;
	*mCur = fgetc(meta);
	*dCur = fgetc(data);

	unsigned char tagSize = 8; // THIS IS INEFFICIENT BUT TIME
	unsigned char numRunSize = 48;

	*numRuns = get(meta, mUsed, mCur, numRunSize);
	*runLen = (char)get(meta, mUsed, mCur, tagSize);
	*keyLen = (char)get(data, dUsed, dCur, tagSize);
}

void decompress(FILE *meta, FILE *data, FILE *out, unsigned char *mUsed,
		unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		unsigned char runLen, unsigned char keyLen, uint64_t numRuns)
{
	// iterates throuh meta to find a run length, then through data for that
	// length. continues for numRuns iterations through meta.
	uint64_t run, key, i, j, k;
	unsigned char oCur;
	unsigned char oUsed = 0;

	for (k = 0; k < numRuns; ++k) {
		run = get(meta, mUsed, mCur, runLen);
		// escape code indicating a series of unique keys
		if (run == 0) {
			run = get(meta, mUsed, mCur, runLen);
			//++k;
			for (j = 0; j < run; ++j) {
				// iterate through unique keys, writing them to the file
				key = get(data, dUsed, dCur, keyLen);
				//printf("key: %" PRIx64 "\n", key);
				put(out, key, &oUsed, &oCur, keyLen);
			}
		}
		// "proper" run (repetition of the same key)
		else {
			key = get(data, dUsed, dCur, keyLen);
			//printf("key: %" PRIx64 "\n", key);
			for (j = 0; j < run; ++j) {
				// write the key as many times as the meta file says to
				put(out, key, &oUsed, &oCur, keyLen);
			}
		}
	}
}
