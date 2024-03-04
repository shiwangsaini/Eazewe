// Chunk.h (sequence of bytecode)
// code representation
#pragma once


#ifndef EAZE_CHUNK_H
#define EAZE_CHUNK_H

#include "common.h"
#include "value.h"

// Operation code
typedef enum {
	OP_RETURN,
} Op_Code;	// defined as Op_code

// dynamic array
typedef struct {
	unsigned int count;
	unsigned int capacity;

	ValueArray constant;
	uint8_t* code;
} Chunk;	// defined as Chunk


void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
int addConstant(Chunk* chunk, Value value);

#endif	// !Eaze_chunk_h
