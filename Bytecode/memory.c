// memory implementation

#include <stdlib.h>

#include "vm.h"
#include "memory.h"


// use for op such as grow, shrink, freeing and allocate on array 
void* reallocate(void* pointer, size_t old_size, size_t new_size) {
    if (new_size == 0){
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, new_size);
    if (result == NULL) exit(1);
    return result;
}

// walking list of objects to free them
static void freeObject(Obj* object) {
    switch (object->type)
    {
    case OBJ_STRING: {
        ObjString* string = (ObjString*)object;
        FREE_ARRAY(char, string->chars, string->length + 1);
        FREE(ObjString, object);
        break;
    }
    }
}

// free Objects when vm finishes for GC
void freeObjects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        freeObjects(object);
        object = next;
    }
}
