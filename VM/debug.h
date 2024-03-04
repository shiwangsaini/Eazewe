// debug.h
#pragma once

#ifndef EAZE_DEBUG_H
#define EAZE_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
unsigned int disassembleInstruction(Chunk* chunk, int offset);
static int simpleInstruction(const char* name, int offset);
static int constantInstruction(const char* name,Chunk* chunk ,int offset);

#endif // !Eaze_debug_h
