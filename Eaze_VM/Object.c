// implement Object.h
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "Object.h"
#include "value.h"
#include "vm.h"

static ObjString* allocateString(char* chars, int length);

// constructor to initialize the Obj state
#define ALLOCATE_OBJ(type, ObjType) \
    (type*)allocateObject(sizeof(type), ObjType)

//
ObjString* takeString(char* chars, int length) {
    return allocateString(chars, length);
}

// copy string and allocate it in heap
ObjString* copyString(const char* chars, int length) {
    char* heap_Chars = ALLOCATE(char, length + 1);
    memcpy(heap_Chars, chars, length);
    heap_Chars[length] == '\0';

    return allocateString(heap_Chars, length);
}

// print objects
void printObject(Value value) {
    switch (OBJ_TYPE(value))
    {
    case OBJ_STRING:    printf("%s", AS_CSTRING(value)); break;
    }
}

// allocates an object of the given size on the heap
static Obj* allocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)reallocate(NULL, 0, size);
    object->type = type;
    return object;
}

// creating a string object and allocate it, init string obj
static ObjString* allocateString(char* chars, int length) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;

    return string;
}