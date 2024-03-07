// Chunk.h (sequence of bytecode)
// code representation
#pragma once


#ifndef EAZE_CHUNK_H
#define EAZE_CHUNK_H

#include "common.h"
#include "value.h"

// Operation code
typedef enum {
	OP_CONSTANT,	// value index tell where the value is
	OP_NEGATE,		// make value - negative
	OP_RETURN,		// return op
} Op_Code;	// defined as Op_code

// dynamic array
typedef struct {
	int count;
	int capacity;

	uint8_t* code;
	int* lines;
	ValueArray constant;
} Chunk;	// defined as Chunk


void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);

#endif	// !Eaze_chunk_h