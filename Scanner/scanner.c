// scanner implementation
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

//
typedef struct {
	const char* start;		// beginning of current lexeme
	const char* current;	// current char being looked at
	int line;				// current lexeme is on line // err purpose
} Scanner;

Scanner scanner;

// init the scanner fields
void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

// scan and provide apropriate tokentype
Token scanToken() {
	skipWhiteSpace();
	scanner.start = scanner.current;

	if (isAtEnd()) makeToken(TOKEN_EOF);

	char c = advance();
	if (isDigit(c)) return number();

	switch (c) {
	case '(': return makeToken(TOKEN_LEFT_PAREN);
	case ')': return makeToken(TOKEN_RIGHT_PAREN);
	case '{': return makeToken(TOKEN_LEFT_BRACE);
	case '}': return makeToken(TOKEN_RIGHT_BRACE);
	case ';': return makeToken(TOKEN_SEMICOLON);
	case ',': return makeToken(TOKEN_COMMA);
	case '.': return makeToken(TOKEN_DOT);
	case '-': return makeToken(TOKEN_MINUS);
	case '+': return makeToken(TOKEN_PLUSE);
	case '/': return makeToken(TOKEN_SLASH);
	case '*': return makeToken(TOKEN_STAR);

	case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
	case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
	case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
	case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);

	case '"': return string();
	default:
		break;
	}

	return errorToken("Unexpected character.");
}

// tells if current is at end or not
static bool isAtEnd() {
	return *scanner.current == '\0';
}

// check if c is a digit
static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

// check the next expected char
static bool match(char expected) {
	if(isAtEnd())return false;
	if (*scanner.current != expected) return false;

	scanner.current++;
	return true;
}

// construct token and give it TokenType
static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	token.line = scanner.line;

	return token;
}

// generate or return error token
static Token errorToken(const char* message) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = message;
	token.length = (int)strlen(message);
	token.line = scanner.line;

	return token;
}

// until " meet termination " its a string
static Token string() {
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n') scanner.line++;
		advance();
	}

	if (isAtEnd()) return errorToken("Unterminated string");
	// the closing quote
	advance();
	return makeToken(TOKEN_STRING);
}

// consume current char and return it
static char advance() {
	scanner.current++;
	return scanner.current[-1];
}

// returns the current character, but doesn’t consume it
static char peek() {
	return *scanner.current;
}
// peek() but return one char past the current
static char peekNext() {
	if (isAtEnd()) return '\0';
	return scanner.current[1];
}

// advances the scanner past any leading whitespace
static void skipWhiteSpace() {
	for (;;) {
		char c = peek();
		switch (c) {
		case ' ':
		case '\r':
		case '\t':
			advance();
			break;
		case '\n':
			scanner.line++;
			advance();
			break;
		case '/':
			if (peekNext() == '/') {
				// comment goes untile the end of line
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else return;
			break;
		default:
			break;
		}
	}
}