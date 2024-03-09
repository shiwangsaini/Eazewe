// compiler implementation
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif // DEBUG_PRINT_CODE

// A Parser struct
typedef struct {
	Token current;		// current token given to
	Token previous;		// previous for stashing token for further use
	bool had_Error;		// is there error?
	bool panic_Mode;	// should panic or not
} Parser;

// Precedence level struct
typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,		// =
	PREC_OR,				// or
	PREC_AND,				// and
	PREC_EQUALITY,			// == , !=
	PREC_COMPARISON,		// < > , <= >=
	PREC_TERM,				// + -
	PREC_FACTOR,			// * /
	PREC_UNARY,				// ! -
	PREC_CALL,				// . ()
	PREC_PRIMARY
} Precedence;

// parse doesn't take arg and return nothing(for function type)
typedef void (*ParseFn) ();

// represent single row in parser table
typedef struct {
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
} ParseRule;

// Parser obj
Parser parser;

// chunk obj
Chunk* compiling_Chunk;

// return the chunk that being compile
static Chunk* currentChunk() {
	return compiling_Chunk;
}

// tells where the error at location
static void errorAt(Token* token, const char* message) {

	// if panic do nothing and leave any other error or just shut off
	if (parser.panic_Mode) return;
	parser.panic_Mode = true;

	fprintf(stderr, "[line : %d] Error", token->line);

	// encounter end of file
	if (token->type == TOKEN_EOF) {
		fprintf(stderr, " at end");
	}
	// handle error
	else if(token->type == TOKEN_ERROR) {
		// nothing yet 
	}
	else {
		fprintf(stderr, " at '%.*s'", token->length, token->start);

	}

	fprintf(stderr, ": %s\n", message);
	parser.had_Error = true;
}

// tells error happened using what? for previous
static void error(const char* message) {
	errorAt(&parser.previous, message);
}

// tells error at current
static void errorAtCurrent(const char* message) {
	errorAt(&parser.current, message);
}

// get tokens and generate error messages
static void advance() {
	parser.previous = parser.current;
	
	// keep reading tokens
	for (;;) {
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;

		errorAtCurrent(parser.current.start);
	}
}

// validates that the token has an expected type
static void consume(TokenType type, const char* message) {
	if (parser.current.type == type) {
		advance();
		return;
	}

	errorAtCurrent(message);
}

// appending a single byte to the chunk
static void emitByte(uint8_t byte) {
	writeChunk(currentChunk(), byte, parser.previous.line);
}
// emit 2 op
static void emitBytes(uint8_t byte1, uint8_t byte2) {
	emitByte(byte1);
	emitByte(byte2);
}

// return op_return
static void emitReturn() {
	emitByte(OP_RETURN);
}

// To insert an entry in the constant table
static uint8_t makeConstant(Value value) {
	int constant = addConstant(currentChunk, value);
	if (constant > UINT8_MAX) {		// if > uint8 max
		error("Too many constant in one chunk");
		return 0;
	}

	return (uint8_t)constant;
}

// emit constant to bytes
static void emitConstant(Value value) {
	emitBytes(OP_CONSTANT, makeConstant(value));
}

// end compile with OP_RETURN
static void endCompiler() {
	emitReturn();
#ifdef DEBUG_PRINT_CODE
	if (!parser.had_Error) {
		disassembleChunk(currentChunk(), "code");
	}
#endif // DEBUG_PRINT_CODE
}

// -------
static void expression();
static ParseRule* getRule(TokenType type);
static void parsePrecedence(Precedence precedence);
// -------

// emit byte on op_type
static void binary() {
	// Remember the operator
	TokenType op_Type = parser.previous.type;

	// Compile the right operand
	ParseRule* rule = getRule(op_Type);
	parsePrecedence((Precedence)(rule->precedence + 1));

	// Emit the operator instruction
	switch (op_Type)
	{
	case TOKEN_PLUSE:		emitByte(OP_ADD); break;
	case TOKEN_MINUS:		emitByte(OP_SUBTRACT); break;
	case TOKEN_STAR:		emitByte(OP_MULTIPY); break;
	case TOKEN_SLASH:		emitByte(OP_DIVIDE); break;
	default:
		return; // Unreachable
	}
}

// parentheses for grouping
static void grouping() {
	expression();
	// to be expected
	consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

//store a pointer at the TOKEN_NUMBER index in the array
static void number(bool can_Assign) {
	// str to double
	double value = strtod(parser.previous.start, NULL);
	emitConstant(value);
}

// Unary negation handling
static void unary() {
	TokenType op_Type = parser.previous.type;

	// Compile the operand
	parsePrecedence(PREC_UNARY);

	// Emit the operator instruction
	switch (op_Type)
	{
	case TOKEN_MINUS: emitByte(OP_NEGATE); break;
	default:
		return; // Unreachable
	}
}

// Parser rule Table
ParseRule rules[] = {

  //[TOKEN_TYPE]			= {fixes,		function,	prec_level}
	[TOKEN_LEFT_PAREN]		= {grouping,	NULL,	PREC_NONE},
	[TOKEN_RIGHT_PAREN]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_LEFT_BRACE]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_RIGHT_BRACE]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_COMMA]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_DOT]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_MINUS]			= {unary,		binary,	PREC_TERM},
	[TOKEN_PLUSE]			= {NULL,		binary,	PREC_TERM},
	[TOKEN_SEMICOLON]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_SLASH]			= {NULL,		binary,	PREC_FACTOR},
	[TOKEN_STAR]			= {NULL,		binary,	PREC_FACTOR},
	[TOKEN_BANG]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_BANG_EQUAL]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_EQUAL]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_EQUAL_EQUAL]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_GREATER]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_GREATER_EQUAL]	= {NULL,		NULL,	PREC_NONE},
	[TOKEN_LESS]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_LESS_EQUAL]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_IDENTIFIER]		= {NULL,		NULL,	PREC_NONE},
	[TOKEN_STRING]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_NUMBER]			= {number,		NULL,	PREC_NONE}, 
	[TOKEN_AND]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_CLASS]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_ELSE]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_FALSE]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_FOR]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_FUN]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_IF]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_NIL]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_OR]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_PRINT]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_RETURN]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_SUPER]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_THIS]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_TRUE]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_VAR]				= {NULL,		NULL,	PREC_NONE},
	[TOKEN_WHILE]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_ERROR]			= {NULL,		NULL,	PREC_NONE},
	[TOKEN_EOF]				= {NULL,		NULL,	PREC_NONE}
};

// parse the lower precedence level
static void parsePrecedence(Precedence precedence) {
	advance();
	// function pointer
	ParseFn prefixRule = getRule(parser.previous.type)->prefix;
	if (prefixRule == NULL) {
		error("Expect expression.");
		return;
	}

	prefixRule();

	while (precedence <= getRule(parser.current.type)->precedence) {
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule();
	}
}

// return which token type has which rule set
static ParseRule* getRule(TokenType type) {
	return &rules[type];
}

// handle expressions(123, -123, +, - etc.) in compiler
static void expression() {
	parsePrecedence(PREC_ASSIGNMENT);
}

// compile the source code and init the scanner
bool compile(const char* source, Chunk* chunk) {
	initScanner(source);
	compiling_Chunk = chunk;

	// init parser
	parser.had_Error = false;
	parser.panic_Mode = false;

	advance();
	expression();
	consume(TOKEN_EOF, "Expected end if expression.");
	endCompiler();
	return !parser.had_Error;		// reverse the had error bool
}