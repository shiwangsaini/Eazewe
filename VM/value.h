// value.h
// stores the values

#pragma once

#ifndef EAZE_VALUE_H
#define EAZE_VALUE_H

#include "common.h"

// D.T double = Value
typedef double Value;

// dynamic array of value
typedef struct {
	unsigned int capacity;
	unsigned int count;
	Value* values;
} ValueArray;	//defined as ValueArray

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif // !EAZE_VALUE_H
