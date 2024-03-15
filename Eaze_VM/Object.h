// object.h // all heap allocated type handled by object
#pragma once

#ifndef Eaze_Object_h
#define Eaze_Object_h

#include "common.h"
#include "value.h"

// extracts obj type
#define OBJ_TYPE(value)			(AS_OBJ(value)->type)
// detect that such a cast is safe
#define IS_STRING(value)			isObjType(value, OBJ_STRING)
// Value that is expected to contain a pointer to a valid
// ObjString on the heap
#define AS_STRING(value)			((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)			(((ObjString*)AS_OBJ(value))->chars)


// enum contains obj types
typedef enum {
	OBJ_STRING,
} ObjType;

// structure of obj
struct Obj {
	ObjType type;
};

// structure of string obj
struct ObjString {
	Obj obj;
	int length;			// length of array
	char* chars;		// array of char
};


// copy string in new obj
ObjString* copyString(const char* chars, int length);

// will tell us when it’s safe to cast a value to a
// specific object type
static inline bool isObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

//
static ObjString* allocateString(char* chars, int length);

#endif // !Eaze_Object_h
