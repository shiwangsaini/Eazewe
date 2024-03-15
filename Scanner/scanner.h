// scanner.h also does tokenizing
#pragma once

#ifndef Eaze_scanner_h
#define Eaze_scanner_h

// Eaze Token
typedef enum {
	// Single-char tokens
	TOKEN_LEFT_PAREN,	// "("
	TOKEN_RIGHT_PAREN,	// ")"
	TOKEN_LEFT_BRACE,	// "["
	TOKEN_RIGHT_BRACE,	// "]"
	TOKEN_COMMA,		// ","
	TOKEN_DOT,			// "."
	TOKEN_MINUS,		// "-"
	TOKEN_PLUSE,		// "+"
	TOKEN_SEMICOLON,	// ";"
	TOKEN_SLASH,		// "/"
	TOKEN_STAR,			// "*"

	// One or two char tokens
	TOKEN_BANG,				// "!"
	TOKEN_BANG_EQUAL,		// "!="
	TOKEN_EQUAL,			// "="
	TOKEN_EQUAL_EQUAL,		// "=="
	TOKEN_GREATER,			// ">"
	TOKEN_GREATER_EQUAL,	// ">="
	TOKEN_LESS,				// "<"
	TOKEN_LESS_EQUAL,		// "<="

	// literals
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
	TOKEN_NUMBER,

	//	Keywords
	TOKEN_AND,
	TOKEN_CLASS,
	TOKEN_ELSE,
	TOKEN_FALSE,
	TOKEN_FOR,
	TOKEN_FUN,
	TOKEN_IF,
	TOKEN_NIL,
	TOKEN_OR,
	TOKEN_PRINT,
	TOKEN_RETURN,
	TOKEN_SUPER,
	TOKEN_THIS,
	TOKEN_TRUE,
	TOKEN_VAR,
	TOKEN_WHILE,

	TOKEN_ERROR,
	TOKEN_EOF
} TokenType;

// Tokenizer struct
typedef struct {
	TokenType type;
	const char* start;
	int length;
	int line;
} Token;

// scanner function
void initScanner(const char* source);
Token scanToken();

// helper function
static bool isAtEnd();
static bool isDigit(char c);
static bool isAlpha(char c);
static bool match(char expected);
static Token makeToken(TokenType type);
static Token errorToken(const char* message);
static Token identifier();
static TokenType identifierType();
static TokenType checkKeyword(int start, int length, const char* rest, TokenType type);
static char peek();
static char peekNext();
static void skipWhiteSpace();

#endif // !Eaze_scanner_h