/* SPDX-License-Identifier: GPL-3.0 */

#ifndef COMP_COMMON
#define COMP_COMMON

#include <inttypes.h>
#include <sys/time.h>
#include <stdio.h>

/**
 * @brief Structure representing a dynamic array of 64-bit unsigned integers.
 *
 * This structure provides functionalities for managing a dynamically resizable
 * array of uint64_t values, including initialization, element access,
 * appending, clearing, and size retrieval.
 */
struct u64array {
	uint64_t *data;         /**< Pointer to the dynamically allocated array data. */
	unsigned long size;     /**< Current allocated size of the array. */
	unsigned long n;        /**< Number of elements currently stored in the array. */
	uint64_t biggest;       /**< The largest value currently stored in the array. */
};

/**
 * @brief Initializes a u64array structure.
 *
 * Allocates initial memory for the array and sets the size and count to zero.
 *
 * @param arr Pointer to the u64array structure to initialize.
 */
void u64array_init(struct u64array *arr);

/**
 * @brief Retrieves an element from the u64array at the specified index.
 *
 * @param arr Pointer to the u64array structure.
 * @param idx Index of the element to retrieve.
 * @param ret Pointer to a uint64_t where the retrieved value will be stored.
 */
void u64array_get(struct u64array *arr, unsigned long idx, uint64_t *ret);

/**
 * @brief Appends a new element to the end of the u64array.
 *
 * If necessary, resizes the array to accommodate the new element.
 *
 * @param arr Pointer to the u64array structure.
 * @param toAdd The uint64_t value to append.
 */
void u64array_push_back(struct u64array *arr, uint64_t toAdd);

/**
 * @brief Clears the u64array, freeing allocated memory and resetting size and count.
 *
 * @param arr Pointer to the u64array structure to clear.
 */
void u64array_clear(struct u64array *arr);

/**
 * @brief Retrieves the current number of elements in the u64array.
 *
 * @param arr Pointer to the u64array structure.
 * @param toPut Pointer to an unsigned long where the size will be stored.
 */
void u64array_size(struct u64array *arr, unsigned long *toPut);

/**
 * @brief Writes a 64-bit unsigned integer to a file in binary format.
 *
 * @param file Pointer to the file to write to.
 * @param toWrite The uint64_t value to write.
 */
void write64ToFile(FILE *file, uint64_t toWrite);

/**
 * @brief Calculates the elapsed time between two timeval structures.
 *
 * Subtracts the start time from the end time and stores the result in the
 * elapsed time structure.
 *
 * @param start Pointer to the start timeval structure.
 * @param end Pointer to the end timeval structure.
 * @param elapsed Pointer to the timeval structure to store the elapsed time.
 */
void subtractTime(struct timeval *start, struct timeval *end,
		  struct timeval *elapsed);

// Tags used for MPI
#define BUFFER_SIZE_TAG 1       /**< MPI tag for buffer size messages. */
#define SEND_BUFFER_TAG 2       /**< MPI tag for buffer data messages. */

#endif // COMP_COMMON
