// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "../include/common.h"

void u64array_init(struct u64array *arr)
{
	arr->n = 0;
	arr->size = 20;
	arr->data = malloc(sizeof(uint64_t) * arr->size);
	arr->biggest = 0;
}

void u64array_get(struct u64array *arr, unsigned long idx, uint64_t *ret)
{
	*ret = arr->data[idx];
}

void u64array_push_back(struct u64array *arr, uint64_t toAdd)
{
	if (arr->n >= arr->size) {
		unsigned long newSize = arr->size * 2;

		// Monster allocation
		uint64_t *temp = malloc(sizeof(uint64_t) * (newSize));

		// Copy all the elements over
		for (unsigned long i = 0; i < arr->size; ++i)
			temp[i] = arr->data[i];

		free(arr->data);
		arr->size = newSize;
		arr->data = temp;
	}

	if (arr->biggest < toAdd)
		arr->biggest = toAdd;

	arr->data[arr->n] = toAdd;
	arr->n = arr->n + 1;
}

void u64array_clear(struct u64array *arr)
{
	arr->n = 0;
}

void u64array_size(struct u64array *arr, unsigned long *toPut)
{
	*toPut = arr->n;
}

void u64array_free(struct u64array *arr)
{
	free(arr->data);
}
