//Implement chunk.h

#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

//initialize Chunk
void initChunk(Chunk* chunk) {
	chunk->count = 0;
	chunk->capacity = 0;
	chunk->code = NULL;
	initValueArray(&chunk->constant);
} // Empty

void freeChunk(Chunk* chunk) {
	FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
	freeValueArray(&chunk->constant);
	initChunk(chunk);
}

// append or add byte at the end of chunk
void writeChunk(Chunk* chunk, uint8_t byte) {
	if (chunk->capacity < chunk->count + 1) {
		unsigned int old_capacity = chunk->capacity;
		chunk->capacity = GROW_CAPACITY(old_capacity);
		chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
	}

	chunk->code[chunk->count] = byte;
	chunk->count++;
}

// add value to chunk
int addConstant(Chunk* chunk, Value value)
{
	writeValueArray(&chunk->constant, value);
	return chunk->constant.count - 1;
}
