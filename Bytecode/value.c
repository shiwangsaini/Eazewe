// Implement value.h

#include <stdio.h>

#include "memory.h"
#include "value.h"

// initialized the value array(VA)
void initValueArray(ValueArray* array) {
	array->values = NULL;
	array->capacity = 0;
	array->count = 0;
}

// write in VA
void writeValueArray(ValueArray* array, Value value) {
	// capacity < count+1, increase the capacity and value from old cap
	if (array->capacity < array->count + 1) {
		 int old_Cap = array->capacity;
		array->capacity = GROW_CAPACITY(old_Cap);
		array->values = GROW_ARRAY(Value, array->values, old_Cap, array->capacity);
	}

	array->values[array->count] = value;
	++array->count;
}

// free VA
void freeValueArray(ValueArray* array) {
	FREE_ARRAY(Value, array->values, array->capacity);
	//initialize Value
	initValueArray(array);
}

// Print Value
void printValue(Value value) {
	printf("%g", value);
}