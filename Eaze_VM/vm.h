// vm.h
#pragma once

#ifndef Eaze_vm_h
#define Eaze_vm_h

#include "chunk.h"
#include "value.h"

#define MAX_STACK 256

// stuct defined as VM
typedef struct {
	Chunk* chunk;
	uint8_t* ip;	// instructor pointer/ PC program counter
	// ip* keep track of current instruction and  points to next instruction
	// through dereferencing it in bytecode array

	// A value stack
	Value stack[MAX_STACK];
	Value* stackTop;	// stack pointer points to biggening of the array
} VM;

// VM responds
typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR,
	INTERPRET_UNEXPECTED_ERROR
} InterpretResult;

// VM functions
void initVM();
void freeVM();

//
InterpretResult interpret(const char* source);
static InterpretResult run();

// Stack functions
static void resetStack();
void push(Value value);
Value pop();
static void runtimeError(const char* format, ...);
static bool isFalsey(Value value);

#endif // !Eaze_vm_h