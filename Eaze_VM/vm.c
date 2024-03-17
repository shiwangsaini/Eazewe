// Implement vm

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "debug.h"
#include "Object.h"
#include "memory.h"
#include "compiler.h"
#include "vm.h"

VM vm;

//
static void concatenate();
static void runtimeError(const char* format, ...);
static Value peek(int distance);
static bool isFalsey(Value value);

// initialize VM
void initVM() {
	resetStack();
	vm.objects = NULL;
	printf("Started EazeWe Virtual Machine > for collage Project [CH.B. Polytechnic].\n");
	printf("In console please write one line of code.\n");
}

// free VM
void freeVM() {
	freeObjects();
	printf("EazeWe Ended.\n");
}

// store executed chunks in VM
InterpretResult interpret(const char* source) {
	Chunk chunk;
	initChunk(&chunk);

	// if compile errors encounter
	if (!compile(source, &chunk)) {		// returns true if compile false
		freeChunk(&chunk);
		printf("interpret !compiled");
		return INTERPRET_COMPILE_ERROR;
	}

	vm.chunk = &chunk;
	vm.ip = vm.chunk->code;

	InterpretResult result = run();

	// when VM finish free the chunk
	freeChunk(&chunk);
	return result;
}

// keep track of next instruction and provide appropiate responds to them
InterpretResult run() {
	printf("in run INTRES\n");
// deref the ip and increment it
#define READ_BYTE() (*vm.ip++)
// read next byte from byte code
#define READ_CONSTANT() (vm.chunk->constant.values[READ_BYTE()])

//
#define BINARY_OP(valueType, op)\
	do { \
		if(!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
			runtimeError("Operands must be numbers."); \
			return INTERPRET_RUNTIME_ERROR; \
		} \
		double b = AS_NUMBER(pop()); \
		double a = AS_NUMBER(pop()); \
		push(valueType(a op b)); \
	} while (false)

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
		case OP_NIL:		push(NIL_VAL); break;
		case OP_TRUE:		push(BOOL_VAL(true)); break;
		case OP_FALSE:		push(BOOL_VAL(false)); break;

		case OP_EQUAL: {
			Value b = pop();
			Value a = pop();
			push(BOOL_VAL(valuesEqual(a, b)));
			break;
		}
		case OP_GREATER:	BINARY_OP(BOOL_VAL, >); break;
		case OP_LESS:		BINARY_OP(BOOL_VAL, <); break;
		case OP_ADD: {
			if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
				concatenate();
			}
			else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
				double b = AS_NUMBER(pop());
				double a = AS_NUMBER(pop());
				push(NUMBER_VAL(a + b));
			}
			else {
				runtimeError("Operands must nums or chars.");
				return INTERPRET_RUNTIME_ERROR;
			}
			break;
		}
		case OP_SUBTRACT:	BINARY_OP(NUMBER_VAL, -); break;
		case OP_MULTIPY:	BINARY_OP(NUMBER_VAL, *); break;
		case OP_DIVIDE:		BINARY_OP(NUMBER_VAL, /); break;
		case OP_NOT:		push(BOOL_VAL(isFalsey(pop()))); break;
		case OP_NEGATE:
			if(!IS_NUMBER(peek(0))) {	// if its a number
				runtimeError("Operand must be a number.");
				return INTERPRET_RUNTIME_ERROR;
			}
			//else
			push(NUMBER_VAL(-AS_NUMBER(pop())));
			break;

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
#undef BINARY_OP
}

// helper func reset the stack
static void resetStack() {
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

// report runtime errors during the execution of the virtual machine (VM)
static void runtimeError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);	// prints the formatted output to the stderr stream 
									// using the format string format and the variable arguments in args
	va_end(args);
	fputs("\n", stderr);		//ensure the error message is terminated with a newline

	size_t instruction = vm.ip - vm.chunk->code - 1;
	int line = vm.chunk->lines[instruction];
	fprintf(stderr, "[line %d] in script\n", line);		//where the error occurred

	resetStack();
}

// access the value at given distance 
// how far down from the top of the stack to look 0 is top 1 is down
static Value peek(int distance) {
	return vm.stackTop[-1 - distance];
}

// nil and false are falsey and every other value
// behaves like true
static bool isFalsey(Value value) {
	return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

// concatenate the string
static void concatenate() {
	ObjString* b = AS_STRING(pop());
	ObjString* a = AS_STRING(pop());

	int length = a->length + b->length;
	char* chars = ALLOCATE(char, length + 1);
	memcpy(chars + a->length, b->chars, b->length);
	chars[length] = '\0';
	ObjString* result = takeString(chars, length);
	push(OBJ_VAL(result));		// push concatenated result to stack
}