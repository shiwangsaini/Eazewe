// Implement debug.h
#include <stdio.h>

#include "value.h"
#include "debug.h"

// machine code to textual list
void disassembleChunk(Chunk* chunk, const char* name) {
    printf("debug -- == %s ==\n", name);

    for (unsigned int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

// Disassemble instruction at given offset
unsigned int disassembleInstruction(Chunk* chunk, int offset) {
    printf("debug -- %04u ", offset);
    if ((offset > 0) && (chunk->lines[offset] == chunk->lines[offset - 1])) {
        printf("    | ");
    }
    else {
        printf("%4d ", chunk->lines[offset]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT__ ", chunk, offset);

    case OP_RETURN:
        return simpleInstruction("OP_RETURN__ ", offset);

    default:
        printf("debug -- Unknown Op_code %d\n", instruction);
        return offset + 1;
    }
}

// Print simple instruction
static int simpleInstruction(const char* name, int offset) {
    printf("debug -- %04d %s\n", offset, name);
    return offset + 1;
}

//
int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s %04d \t &Value = '", name, constant);
    printValue(chunk->constant.values[constant]);
    printf("'\n");
    return offset + 2;
}