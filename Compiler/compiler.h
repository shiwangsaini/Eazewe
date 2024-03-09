// compile
#pragma once

#ifndef Eaze_compiler_h
#define Eaze_compiler_h

#include "vm.h"

// compiler function
bool compile(const char* source, Chunk* chunk);
static void error(const char* message);
static void errorAtCurrent(const char* message);
static void advance();

#endif // !Eaze_compiler_h
