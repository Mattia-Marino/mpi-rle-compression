/* SPDX-License-Identifier: GPL-3.0 */

#ifndef U64ARRAY_H
#define U64ARRAY_H

#include <inttypes.h>
#include <stdio.h>

#include "common.h"

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
void u64array_get(struct u64array *arr, size_t idx, uint64_t *ret);

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
 * @brief Clears the u64array, resetting the number of elements to zero.
 *
 * Note: This does not free the allocated memory. Use u64array_free to release memory.
 *
 * @param arr Pointer to the u64array structure to clear.
 */
void u64array_clear(struct u64array *arr);

/**
 * @brief Retrieves the current number of elements in the u64array.
 *
 * @param arr Pointer to the u64array structure.
 * @param toPut Pointer to a size_t where the size will be stored.
 */
void u64array_size(struct u64array *arr, size_t *toPut);

/**
 * @brief Frees the dynamically allocated memory associated with the u64array.
 *
 * @param arr Pointer to the u64array structure.
 */
void u64array_free(struct u64array *arr);

#endif // U64ARRAY_H
