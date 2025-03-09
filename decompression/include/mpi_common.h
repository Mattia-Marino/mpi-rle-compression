/* SPDX-License-Identifier: GPL-3.0 */

#ifndef MPI_COMMON_H
#define MPI_COMMON_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

// Macro to find the minimum of two values
#define min(a, b) (((a) < (b)) ? (a) : (b))

/**
 * @brief Reads a variable-length integer from a file stream.
 *
 * @param stream File stream to read from.
 * @param used Flag indicating if the current buffer 'cur' is used.
 * @param cur Current byte buffer.
 * @param size Maximum size of the variable-length integer in bytes.
 * @return The read integer.
 */
uint64_t get(FILE *stream, unsigned char *used, unsigned char *cur,
	     unsigned char size);

/**
 * @brief Writes a variable-length integer to a buffer.
 *
 * @param outBuf Output buffer to write to.
 * @param toPut Integer to write.
 * @param used Flag indicating if the current buffer 'cur' is used.
 * @param cur Current byte buffer.
 * @param size Maximum size of the variable-length integer in bytes.
 * @param bufIdx Current index in the output buffer.
 */
void put(unsigned char *outBuf, uint64_t toPut, unsigned char *used,
	 unsigned char *cur, unsigned char size, uint64_t *bufIdx);

/**
 * @brief Calculates the time difference between two timeval structs.
 *
 * @param start Starting time.
 * @param end Ending time.
 * @param elapsed Timeval struct to store the elapsed time.
 */
void subtractTime(struct timeval *start, struct timeval *end,
		  struct timeval *elapsed);

#endif // MPI_COMMON_H
