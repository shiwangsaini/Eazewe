// Implement debug.h
#include <stdio.h>

#include "debug.h"

// machine code to textual list
void disassembleChunk(Chunk* chunk, const char* name) {
    printf("debug -- == %s ==\n", name);

    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset);
    }
}

// Print simple instruction
static int simpleInstruction(const char* name, int offset) {
    printf("debug -- %04d %s\n", offset, name);
    return offset + 1;
}

// Disassemble instruction at given offset
int disassembleInstruction(Chunk* chunk, int offset) {
    printf("debug -- %04d ", offset);

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
    case OP_RETURN:
        return simpleInstruction("OP_RETURN__", offset);
    default:
        printf("debug -- Unknown Op_code %d\n", instruction);
        return offset + 1;
    }
}

