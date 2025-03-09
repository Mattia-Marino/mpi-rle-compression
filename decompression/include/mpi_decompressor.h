/* SPDX-License-Identifier: GPL-3.0 */

#ifndef MPI_DECOMPRESSOR_H
#define MPI_DECOMPRESSOR_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief Retrieves metadata from the meta and data files.
 *
 * This function reads the initial bytes from the meta and data files to
 * determine the bit lengths of keys, the number of runs, and other
 * essential parameters for decompression.
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
 * @param expProcs Pointer to store the expected number of processes.
 */
void getMetaData(FILE *meta, FILE *data, unsigned char *mUsed,
		 unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		 unsigned char *runLen, unsigned char *keyLen,
		 uint64_t *numRuns, unsigned char *expProcs);

/**
 * @brief Decompresses the data using the provided metadata.
 *
 * This function iterates through the meta file to determine run lengths and
 * the data file to extract keys. It writes the decompressed data into the
 * output buffer.
 *
 * @param meta File pointer to the meta file.
 * @param data File pointer to the data file.
 * @param outBuf Output buffer to store the decompressed data.
 * @param mUsed Pointer to a flag indicating if the current byte in meta is used.
 * @param dUsed Pointer to a flag indicating if the current byte in data is used.
 * @param mCur Pointer to the current byte buffer for meta.
 * @param dCur Pointer to the current byte buffer for data.
 * @param runLen Bit length of the run.
 * @param keyLen Bit length of the key.
 * @param numRuns Number of runs to process.
 */
void decompress(FILE *meta, FILE *data, char *outBuf, unsigned char *mUsed,
		unsigned char *dUsed, unsigned char *mCur, unsigned char *dCur,
		unsigned char runLen, unsigned char keyLen, uint64_t numRuns);

#endif // MPI_DECOMPRESSOR_H
