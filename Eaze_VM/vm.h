// vm.h
#pragma once

#ifndef EAZE_VM_H
#define EAZE_VM_H

#include "chunk.h"

// stuct defined as VM
typedef struct {
	Chunk* chunk;
	uint8_t* ip;
} VM;

// VM responds
typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);

#endif // !EAZE_VM_H