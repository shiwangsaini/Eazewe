// Implement vm

#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

// VM struct obj
VM vm;

// initialize VM
void initVM() {
	resetStack();
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

// if defined disassemble instruction
#ifdef DEBUG_TRACE_EXECUTION
		// show current content of the stack by traversing
		printf("		");
		for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
			printf("[ ");
			printValue(*slot);
			printf(" ]");
		}
		printf("\n");
		disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif // DEBUG_TRACE_EXECUTION

		uint8_t instruction = 0;
		switch (instruction = READ_BYTE())
		{
		case OP_CONSTANT: {
			Value constant = READ_CONSTANT();
			push(constant);
			break;
		}
		case OP_NEGATE: {
			push(-pop());
			break;
		}
		case OP_RETURN: {
			printValue(pop());
			printf("\n");
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

// helper func reset the stack
void resetStack() {
	vm.stackTop = vm.stack;
}

// push new values to stack
void push(Value value) {
	*vm.stackTop = value;	// set value to provided address
	vm.stackTop++;			// then take one step forward
}

// pop recent value in stack
Value pop() {
	vm.stackTop--;			// move pointer back
	return *vm.stackTop;	// then return pointer to new recent stack top
}
