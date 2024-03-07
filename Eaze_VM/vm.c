// Implement vm

#include <stdio.h>

#include "common.h"
#include "vm.h"

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
