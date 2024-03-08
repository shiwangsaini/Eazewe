// value.h
// stores the values

#pragma once

#ifndef Eaze_value_h
#define Eaze_value_h

#include "common.h"

// D.T double = Value
typedef double Value;

// dynamic array of value
typedef struct {
	int capacity;
	int count;
	Value* values;
} ValueArray;	//defined as ValueArray

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif // !Eaze_value_h