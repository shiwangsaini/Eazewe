#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"


// Read eval print loop
static void repl() {
	char line[1024];
	for (;;) {
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}
		interpret(line);
	}
}

//
static char* readFile(const char* path) {
	FILE* file = fopen(path, "rb");
	if (file == NULL) {				// if, throw err
		fprintf(stderr, "Could not open file \"%s\".\n", path);
		exit(74);
	}

	fseek(file, 0L, SEEK_END); 
	size_t file_Size = ftell(file);
	rewind(file);

	char* buffer = (char*)malloc(file_Size + 1);
	if (buffer == NULL) {			// if, throw err
		fprintf(stderr, "Not enough mem to read \"%s\".\n", path);
		exit(74);
	}

	size_t bytes_Read = fread(buffer, sizeof(char), file_Size, file);
	if (bytes_Read < file_Size) {	// if, throw err
		fprintf(stderr, "could not read file \"%s\".\n", path);
		exit(74);
	}

	buffer[bytes_Read] = '\0';

	fclose(file);
	return buffer;
}

//
static void runFile(const char* path) {
	char* source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR) exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}


int main(int argc, const char* argv[]) {
	initVM();

	if (argc == 1) {
		repl();
	}
	else if(argc == 2) {
		runFile(argv[1]);
	}
	else
	{
		fprintf(stderr, "Usage: EazeWe [path]\n");
		exit(64);
	}

	freeVM();

	return 0;
}