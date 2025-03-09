/* SPDX-License-Identifier: GPL-3.0 */

#ifndef COMMON_H
#define COMMON_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

// Macro to find the minimum of two values
#define min(a, b) (((a) < (b)) ? (a) : (b))

/**
 * @brief Function to read a variable-length integer from a file stream.
 *
 * @param stream The file stream to read from.
 * @param used A pointer to a flag indicating if the current buffer 'cur' is used.
 * @param cur A pointer to the current byte buffer.
 * @param size The maximum size of the variable-length integer in bytes.
 * @return uint64_t The variable-length integer read from the stream.
 */
uint64_t get(FILE *stream, unsigned char *used, unsigned char *cur,
	     unsigned char size);

/**
 * @brief Function to write a variable-length integer to a file stream.
 *
 * @param stream The file stream to write to.
 * @param toPut The integer to write.
 * @param used A pointer to a flag indicating if the current buffer 'cur' is used.
 * @param cur A pointer to the current byte buffer.
 * @param size The maximum size of the variable-length integer in bytes.
 */
void put(FILE *stream, uint64_t toPut, unsigned char *used, unsigned char *cur,
	 unsigned char size);

/**
 * @brief Function to calculate the time difference between two timeval structs.
 *
 * @param start The starting time.
 * @param end The ending time.
 * @param elapsed A pointer to a timeval struct to store the elapsed time.
 */
void subtractTime(struct timeval *start, struct timeval *end,
		  struct timeval *elapsed);

#endif // COMMON_H
