// memory.h
// does low-level operation such as manipulating array size etc.
#pragma once

#ifndef Eaze_memory_h
#define Eaze_memory_h

#include "common.h"
#include "object.h"

// allocate a new array on the heap
#define ALLOCATE(type, count) \
	(type*)reallocate(NULL, 0, sizeof(type) * (count))

// grow the capacity by 2 if cap < 8
#define GROW_CAPACITY(capacity) \
	(capacity < 8 ? 8: (capacity * 2))

// grow the size of array to desired cap
#define GROW_ARRAY(type, pointer, old_count, new_count) \
	(type*)reallocate(pointer, sizeof(type) * (old_count), \
		sizeof(type) * (new_count))

// free the array
#define FREE_ARRAY(type, pointer, old_count) \
	reallocate(pointer, sizeof(type) * (old_count), 0)

// free and reallocate pointer with new size of 0
#define FREE(type, pointer) reallocate(pointer, sizeof(type), 0);

void* reallocate(void* pointer, size_t old_size, size_t	new_size);
void freeObjects();

#endif // !Eaze_memory_h