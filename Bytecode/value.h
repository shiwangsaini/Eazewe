// value.h
// stores the values

#pragma once

#ifndef Eaze_value_h
#define Eaze_value_h

#include "common.h"

// Value or datatypes
typedef enum {
	VAL_BOOL,
	VAL_NIL,
	VAL_NUMBER,
} ValueType;

// Datatypes as Value
typedef struct {
	ValueType type;			// 4 byte for type
	// 4 byte padding
	union {					// 1 byte for bool and 7 bytes for double
		bool boolean;
		double number;
	} as;
} Value;		// total of 16 bytes

// These macros return true if the value has that type
#define IS_BOOL(value)			((value).type == VAL_BOOL)
#define IS_NIL(value)			((value).type == VAL_BOOL)
#define IS_NUMBER(value)		((value).type == VAL_BOOL)
// Given a Value of the right type, they
// unwrap it and return the corresponding raw C value
#define AS_BOOL(value)		((value).as.boolean)
#define AS_NUMBER(value)	((value).as.number)
// takes a C value of the appropriate type and produces a Value
// that has the correct type tag and contains the underlying value
#define BOOL_VAL(value)		((Value) {VAL_BOOL, {.boolean = value}})
#define NIL_VAL				((Value) {VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)	((Value) {VAL_NUMBER ,{.number = value}})

// D.T double = Value
//typedef double Value;

// dynamic array of value
typedef struct {
	int capacity;
	int count;
	Value* values;
} ValueArray;	//defined as ValueArray

//
bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif // !Eaze_value_h