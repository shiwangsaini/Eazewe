// debug.h
#pragma once

#ifndef EAZE_DEBUG_H
#define EAZE_DEBUG_H

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);
static int simpleInstruction(const char* name, int offset);

#endif // !Eaze_debug_h
