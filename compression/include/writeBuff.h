/* SPDX-License-Identifier: GPL-3.0 */

#ifndef WRITE_BUFF
#define WRITE_BUFF

#include <inttypes.h>
#include <stdio.h>

/**
 * @brief Structure for buffering and writing 64-bit data to a file based on a key size.
 *
 * This structure manages a buffer and writes data to a file, handling bit-level
 * operations based on the specified key size. It efficiently stores and writes
 * data, ensuring that only the relevant bits are written to the file.
 */
struct writeBuff {
	FILE *file;             /**< Pointer to the file for writing. */
	unsigned int keySize;   /**< Number of bits to write for each element (key size). */
	unsigned int currBit;   /**< Current bit position within the buffer. */
	uint64_t buff;		/**< Buffer to hold data before writing to file. */
};

/**
 * @brief Initializes a writeBuff structure.
 *
 * Sets up the write buffer with the given file and key size, initializing the
 * current bit position and buffer.
 *
 * @param wBuff Pointer to the writeBuff structure to initialize.
 * @param file Pointer to the file to write to.
 * @param keySize Number of bits to write for each element (key size).
 */
void initWriteBuff(struct writeBuff *wBuff, FILE *file, unsigned int keySize);

/**
 * @brief Pushes a 64-bit value to the write buffer.
 *
 * Writes the specified number of bits (key size) from the given value to the
 * buffer. When the buffer is full, it writes the buffered data to the file.
 *
 * @param wBuff Pointer to the writeBuff structure.
 * @param toWrite The 64-bit value to write to the buffer.
 */
void pushToWriteBuff(struct writeBuff *wBuff, uint64_t toWrite);

/**
 * @brief Closes the write buffer and flushes any remaining data to the file.
 *
 * Ensures that all buffered data is written to the file and that the file is
 * properly closed.
 *
 * @param wBuff Pointer to the writeBuff structure.
 */
void closeWriteBuff(struct writeBuff *wBuff);

#endif // WRITE_BUFF
