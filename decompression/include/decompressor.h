/* SPDX-License-Identifier: GPL-3.0 */

#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Retrieves metadata from the meta and data files.
 *
 * This function reads the initial bytes from the meta and data files to
 * determine the bit lengths of keys and the number of runs.
 *
 * @param meta File pointer to the meta file.
 * @param data File pointer to the data file.
 * @param mUsed Pointer to a flag indicating if the current byte in meta is used.
 * @param dUsed Pointer to a flag indicating if the current byte in data is used.
 * @param mCur Pointer to the current byte buffer for meta.
 * @param dCur Pointer to the current byte buffer for data.
 * @param runLen Pointer to store the bit length of the run.
 * @param keyLen Pointer to store the bit length of the key.
 * @param numRuns Pointer to store the number of runs.
 */
void getMetaData(FILE *meta, FILE *data, unsigned char *mUsed,
		 unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		 unsigned char *runLen, unsigned char *keyLen,
		 uint64_t *numRuns);

/**
 * @brief Decompresses the data using the provided metadata.
 *
 * This function iterates through the meta file to determine run lengths and
 * the data file to extract keys. It writes the decompressed data into the
 * output file.
 *
 * @param meta File pointer to the meta file.
 * @param data File pointer to the data file.
 * @param out File pointer to the output file.
 * @param mUsed Pointer to a flag indicating if the current byte in meta is used.
 * @param dUsed Pointer to a flag indicating if the current byte in data is used.
 * @param mCur Pointer to the current byte buffer for meta.
 * @param dCur Pointer to the current byte buffer for data.
 * @param runLen Bit length of the run.
 * @param keyLen Bit length of the key.
 * @param numRuns Number of runs to process.
 */
void decompress(FILE *meta, FILE *data, FILE *out, unsigned char *mUsed,
		unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		unsigned char runLen, unsigned char keyLen, uint64_t numRuns);

#endif // DECOMPRESSOR_H
