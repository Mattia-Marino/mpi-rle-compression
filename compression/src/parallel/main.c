// SPDX-License-Identifier: GPL-3.0

#include <inttypes.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "../../include/buffIter.h"
#include "../../include/common.h"
#include "../../include/compressor.h"
#include "../../include/writeBuff.h"

int main(int argc, char **argv)
{
	int MYRANK, NUMPROCS;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MYRANK);
	MPI_Comm_size(MPI_COMM_WORLD, &NUMPROCS);

	// Master rank starts the timer
	struct timeval tvStart, tvEnd;

	if (MYRANK == MASTER_RANK)
		gettimeofday(&tvStart, 0);

	// Variables used by master
	char *inputFileName, **dataFileNames, **metaFileNames;
	unsigned char *fileBuffer;
	unsigned long bufferSize, inputFileSize;
	FILE *inputFile;

	// Variables used by workers
	unsigned long recvBufferSize;

	// Variables used by everyone
	MPI_Status status;
	char *dataFileName, *metaFileName;
	unsigned long myBufferSize;
	unsigned char *myBuffer;
	unsigned int keySize;
	FILE *myDataFile, *myMetaFile;
	struct u64array counts;
	size_t cutoff;

        static const char data[5] = ".data";
	static const char meta[5] = ".meta";
        
	// Used to convert integers to strings
	char num[20];

	// Master checks if all arguments are there
	if (MYRANK == MASTER_RANK) {
		if (argc != 3) {
			printf("Usage: %s <input file> <key size>\n", argv[0]);
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}

	// Do some input argument setup, assuming their validity
	inputFileName = argv[1];

	// Get the number of chars to take before the first '.' of the given
	// inputFileName
	cutoff = (strchr(inputFileName, '.')) - &(inputFileName[0]);

	// Get the key size
	char *endptr;
	long temp = strtol(argv[2], &endptr, 10);

	if (*endptr != '\0' || argv[2][0] == '\0' || temp < 1 || temp > 64) {
		fprintf(stderr, "Invalid key size format\n");
		MPI_Abort(MPI_COMM_WORLD, -1);
	}
	keySize = (unsigned int)temp;

	// Master does some validation tests
	if (MYRANK == MASTER_RANK) {
		if (keySize < 1 || keySize > 64) {
			fprintf(stderr,
				"Invalid key size \"%d\"; must be in range of [1, 64]\n",
				keySize);
			MPI_Abort(MPI_COMM_WORLD, -1);
		}

		if (NUMPROCS < 1) {
			fprintf(stderr, "Invalid number of processes \"%d\"\n",
				NUMPROCS);
			MPI_Abort(MPI_COMM_WORLD, -1);
		}

		// Print some updates for the user
		printf("Reading with keySize of %d bits\n", keySize);
		printf("Number of processes = %d\n", NUMPROCS);

		// Open the input file and check if it exists
		inputFile = fopen(inputFileName, "rb");

		if (!inputFile) {
			fprintf(stderr, "Error opening input file \"%s\"\n",
				inputFileName);
			MPI_Abort(MPI_COMM_WORLD, -1);
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// Get the number for our filename
	sprintf(num, "%u", MYRANK);

	// Calculate my .data file and my .meta file names
	dataFileName =
		(char *)malloc(sizeof(char) * (cutoff + 6 + strlen(num)));
	metaFileName =
		(char *)malloc(sizeof(char) * (cutoff + 6 + strlen(num)));

	dataFileName[cutoff + 5 + strlen(num)] = '\0';
	metaFileName[cutoff + 5 + strlen(num)] = '\0';

	memmove(dataFileName, inputFileName, cutoff);
	memmove(metaFileName, inputFileName, cutoff);

	// Then copy the number string
	for (int j = 0; j < strlen(num); ++j) {
		dataFileName[cutoff + j] = num[j];
		metaFileName[cutoff + j] = num[j];
	}

	// Now copy the .data and .meta
	for (int j = 0; j < 5; ++j) {
		dataFileName[cutoff + strlen(num) + j] = data[j];
		metaFileName[cutoff + strlen(num) + j] = meta[j];
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (MYRANK == MASTER_RANK) {
		// Get the size of the input file
		inputFileSize = getFileSize(inputFileName);
		printf("Input file size: %lu\n", inputFileSize);

		// Calculate buffer size
		bufferSize = inputFileSize / NUMPROCS;
		myBufferSize = bufferSize;

		// Send buffer size to everyone except last rank, who will get
		// buff + remainder
		for (int i = 1; i < NUMPROCS - 1; i++) {
			MPI_Send(&bufferSize, 1, MPI_UNSIGNED_LONG, i,
				 BUFFER_SIZE_TAG, MPI_COMM_WORLD);
		}

		// Send buffer size to last rank
		bufferSize += inputFileSize % NUMPROCS;
		MPI_Send(&bufferSize, 1, MPI_UNSIGNED_LONG, NUMPROCS - 1,
			 BUFFER_SIZE_TAG, MPI_COMM_WORLD);
	} else {
		MPI_Recv(&recvBufferSize, 1, MPI_UNSIGNED_LONG, MASTER_RANK,
			 BUFFER_SIZE_TAG, MPI_COMM_WORLD, &status);
		myBufferSize = recvBufferSize;
	}

	// Everybody sets up their own buffer
	myBuffer = malloc(sizeof(unsigned char) * myBufferSize);
	if (!myBuffer) {
		fprintf(stderr, "Error allocating buffer for rank %d\n",
			MYRANK);
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	// Wait for all processes to set their buffer
	MPI_Barrier(MPI_COMM_WORLD);

	if (MYRANK == MASTER_RANK) {
		// Read the input
		fileBuffer = malloc(sizeof(unsigned char) * bufferSize);

		if (!fileBuffer) {
			fprintf(stderr,
				"Error allocating read buffer, not enough memory!\n");
			MPI_Abort(MPI_COMM_WORLD, -1);
		}

		// Save 1 element for later. Possibly use fseek later.
		fread(fileBuffer, myBufferSize, 1, inputFile);

		// Read and send a buffer to every process except the last one
		for (int i = 1; i < NUMPROCS - 1; i++) {
			fread(fileBuffer, myBufferSize, 1, inputFile);

			MPI_Send(fileBuffer, myBufferSize, MPI_UNSIGNED_CHAR, i,
				 SEND_BUFFER_TAG, MPI_COMM_WORLD);
		}

		// Read and send a buffer to the last process
		if (NUMPROCS > 1) {
			fread(fileBuffer, bufferSize, 1, inputFile);

			MPI_Send(fileBuffer, bufferSize, MPI_UNSIGNED_CHAR,
				 NUMPROCS - 1, SEND_BUFFER_TAG, MPI_COMM_WORLD);
		}

		// Go back to the beginning of the file and read the first
		// buffer
		fseek(inputFile, 0, SEEK_SET);
		fread(fileBuffer, myBufferSize, 1, inputFile);

		// Setup my buffer info
		myBuffer = fileBuffer;
	} else {
		// Receive the buffer
		MPI_Recv(myBuffer, myBufferSize, MPI_UNSIGNED_CHAR, MASTER_RANK,
			 SEND_BUFFER_TAG, MPI_COMM_WORLD, &status);
	}

	// Calculate my portion of the work
	struct buffIter myIter;
	struct writeBuff metaWriter;
	struct writeBuff dataWriter;
	unsigned long lastPos = 0;
	unsigned long unusedBits = 0;

	uint64_t next = 0;
	uint64_t last = 0;
	uint64_t count = 1;

	myDataFile = fopen(dataFileName, "wb");
	myMetaFile = fopen(metaFileName, "wb");

	initDataFile(myDataFile, keySize);
	initWriteBuff(&dataWriter, myDataFile, keySize);
	initBuffIter(&myIter, myBuffer, myBufferSize, keySize);
	u64array_init(&counts);

	advance(&myIter, &next);
	last = next;

	// Go through the buffer
	while (iterHasNext(&myIter)) {
		advance(&myIter, &next);

		// If we have a match, keep running
		if (next == last)
			++count;
		// If they don't match, write 'last' and 'count' to our files
		else {
			u64array_push_back(&counts, count);
			pushToWriteBuff(&dataWriter, last);

			count = 1;
		}

		last = next;
	}

	// Here we account for unused bits from the buffer
	// This should actually be some wonky number
	unusedBits = unusedBuffBits(&myIter);

	closeWriteBuff(&dataWriter);

	// Now write to the meta file
	unsigned int numBits = 64;

	for (unsigned int i = 0; i < 64; ++i) {
		if ((counts.biggest << i) & 0x8000000000000000) {
			numBits = 64 - i;
			break;
		}
	}

	// Write the num of bits to the meta file
	initMetaFile(myMetaFile, numBits, counts.n, NUMPROCS);

	// Now write the array elements to the meta file at the given bit level
	// Create chars of the elements.

	// For each array element convert it to the numBits format,
	// string them together to then write them as chars.
	initWriteBuff(&metaWriter, myMetaFile, numBits);

	for (unsigned long i = 0; i < counts.n; ++i)
		pushToWriteBuff(&metaWriter, counts.data[i] << (64 - numBits));

	closeWriteBuff(&metaWriter);

	// Stop it all
	MPI_Barrier(MPI_COMM_WORLD);

	// Free all the memory and close files
	free(myBuffer);

	free(counts.data);
	free(dataFileName);
	free(metaFileName);

	fclose(myDataFile);
	fclose(myMetaFile);

	// Close input file and say final time
	if (MYRANK == MASTER_RANK) {
		// Close the input file
		fclose(inputFile);

		// Print elapsed time
		struct timeval elapsedTime;

		gettimeofday(&tvEnd, 0);
		subtractTime(&tvStart, &tvEnd, &elapsedTime);

		printf("Elapsed time: %ld.%ld06\n", elapsedTime.tv_sec,
		       elapsedTime.tv_usec);
	}

	// Finalize MPI
	MPI_Finalize();
	return 0;
}
