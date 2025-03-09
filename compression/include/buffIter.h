/* SPDX-License-Identifier: GPL-3.0 */

#ifndef BUFF_ITER
#define BUFF_ITER

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Structure for iterating through a buffer bit-by-bit.
 *
 * This structure holds the necessary information to traverse a byte buffer,
 * extracting bits based on a specified step size (key size).
 */
struct buffIter {
	unsigned char *buff;            /**< Pointer to the buffer being iterated. */
	unsigned long buffSize;         /**< Size of the buffer in bytes. */
	unsigned long currBit;          /**< Current bit position within the buffer. */
	unsigned long stepSize;         /**< Number of bits to extract in each step (key size). */
};

/**
 * @brief Initializes a buffIter structure.
 *
 * Sets up the iterator to traverse the given buffer, starting from the
 * beginning, with the specified bit step size.
 *
 * @param iter Pointer to the buffIter structure to initialize.
 * @param buffer Pointer to the buffer to iterate through.
 * @param bufferSize Size of the buffer in bytes.
 * @param bitStepSize Number of bits to extract in each step (key size).
 */
void initBuffIter(struct buffIter *iter, unsigned char *buffer,
		  unsigned long bufferSize, unsigned long bitStepSize);

/**
 * @brief Sets the starting bit offset for the iterator.
 *
 * Allows the iterator to start from a specific bit position within the buffer.
 *
 * @param iter Pointer to the buffIter structure.
 * @param startBitOffset Bit position to start iteration from.
 */
void setStartOffset(struct buffIter *iter, unsigned long startBitOffset);

/**
 * @brief Checks if there are more bits to extract from the buffer.
 *
 * Determines whether the iterator has reached the end of the buffer based on
 * the current bit position and step size.
 *
 * @param iter Pointer to the buffIter structure.
 * @return true if there are more bits to extract, false otherwise.
 */
bool iterHasNext(struct buffIter *iter);

/**
 * @brief Extracts the next sequence of bits from the buffer.
 *
 * Advances the iterator and retrieves the next sequence of bits based on the
 * step size.
 *
 * @param iter Pointer to the buffIter structure.
 * @param result Pointer to a uint64_t where the extracted bits will be stored.
 */
void advance(struct buffIter *iter, uint64_t *result);

/**
 * @brief Calculates the number of unused bits at the end of the buffer.
 *
 * Determines how many bits at the end of the buffer were not fully utilized
 * based on the step size.
 *
 * @param iter Pointer to the buffIter structure.
 * @return Number of unused bits.
 */
unsigned long unusedBuffBits(struct buffIter *iter);

#endif // BUFF_ITER
