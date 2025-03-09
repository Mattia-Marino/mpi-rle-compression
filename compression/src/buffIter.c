// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/buffIter.h"
#include "../include/common.h"

// Used to setup a buffer iterator
// Range of bitStepSize = [1,64]
void initBuffIter(struct buffIter *iter, unsigned char *buffer,
		  unsigned long bufferSize, unsigned long bitStepSize)
{
	iter->buff = buffer;
	iter->buffSize = bufferSize;
	iter->stepSize = bitStepSize;
	iter->currBit = 0;
}

void setStartOffset(struct buffIter *iter, unsigned long startBitOffset)
{
	iter->currBit = startBitOffset;
}

bool iterHasNext(struct buffIter *iter)
{
	// If we can advance in the bits
	return ((iter->currBit + (iter->stepSize)) <=
		(iter->buffSize) * 8 + iter->stepSize);
}

void advance(struct buffIter *iter, uint64_t *result)
{
	// If we can advance in the bits, then do so. I think I fixed this...
	if ((iter->currBit + (iter->stepSize)) <=
	    (iter->buffSize) * 8 + (iter->stepSize)) {
		// Current bit and next bit are guaranteed to be in the same
		// uint64_t because the keySize is guaranteed to be <= 64

		unsigned char *ptr = &(iter->buff[iter->currBit / 8]);
		uint64_t container = 0;

		// Now, for the next 8 bytes, copy them into container
		container = (((uint64_t)(ptr[0])) << (56));
		container += (((uint64_t)(ptr[1])) << (48));
		container += (((uint64_t)(ptr[2])) << (40));
		container += (((uint64_t)(ptr[3])) << (32));
		container += (((uint64_t)(ptr[4])) << (24));
		container += (((uint64_t)(ptr[5])) << (16));
		container += (((uint64_t)(ptr[6])) << (8));
		container += (((uint64_t)(ptr[7])) << (0));

		unsigned int nextBitInCntnr =
			(iter->currBit % 8) + (iter->stepSize);

		// Shift to the right
		container = (container >> (64 - nextBitInCntnr));

		// Then shift back to the left
		container = (container << (64 - iter->stepSize));

		*result = container;

		iter->currBit += iter->stepSize;
	}
}

unsigned long unusedBuffBits(struct buffIter *iter)
{
	return (8 * iter->buffSize) - (iter->currBit);
}
