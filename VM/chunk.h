// Chunk.h (sequence of bytecode)
// code representation
#pragma once


#ifndef Eaze_chunk_h
#define Eaze_chunk_h

#include "common.h"

// Operation code
typedef enum {
	OP_RETURN,
} Op_Code;	// defined as Op_code

// dynamic array
typedef struct {
	unsigned int count;
	unsigned int capacity;

	uint8_t* code;
} Chunk;	// defined as Chunk


void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);

#endif	// !Eaze_chunk_h
