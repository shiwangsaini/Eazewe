// Implement vm

#include <stdio.h>

#include "common.h"
#include "vm.h"

// VM struct obj
VM vm;

// initialize VM
void initVM() {
	printf("initialized vm\n");
}

// free VM
void freeVM() {
	printf("freed vm\n");
}

// store executed chunks in VM
InterpretResult interpret(Chunk* chunk) {
	vm.chunk = chunk;
	vm.ip = vm.chunk->code;

	// internal helper runs the bytecode instruction
	return run();
}

// keep track of next instruction and provide appropiate responds to them
InterpretResult run() {

// deref the ip and increment it
#define READ_BYTE() (*vm.ip++)
// read next byte from byte code
#define READ_CONSTANT() (vm.chunk->constant.values[READ_BYTE()])

	// give response
	for (;;) {
		uint8_t instruction;
		switch (instruction = READ_BYTE())
		{
		case OP_CONSTANT: {
			Value constant = READ_CONSTANT();
			printValue(constant);
			printf("\n");
			break;
		}
		case OP_RETURN: {
			return INTERPRET_OK;
		}
		default: {
			printf("Unexpected vm instruction\n");
			return INTERPRET_UNEXPECTED_ERROR;
		}
		}
	}

// undefined macros
#undef READ_BYTE
#undef READ_CONSTANT
}