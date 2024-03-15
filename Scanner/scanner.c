// scanner implementation
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

static Token string();
static Token number();
static char advance();

// structure of scanner
typedef struct {
	const char* start;		// beginning of current lexeme
	const char* current;	// current char being looked at
	int line;				// current lexeme is on line // err purpose
} Scanner;

Scanner scanner;

// init the scanner fields
void initScanner(const char* source) {
	printf("in init scanner\n");

	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

// scan and provide appropriate token type
Token scanToken() {

	skipWhiteSpace();
	scanner.start = scanner.current;

	if (isAtEnd()) makeToken(TOKEN_EOF);

	char c = advance();

	if (isAlpha(c)) return identifier();
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

// check if c is alphabet
bool isAlpha(char c) {
	return (c >= 'A' && c <= 'Z') || 
		   (c >= 'a' && c <= 'z') || 
			c == '_';
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

// Tokenize numbers
static Token number() {
	while (isDigit(peek())) advance();

	// look for fractional part
	if (peek() == '.' && isDigit(peekNext())) {
		// consume the "."
		advance();
		while (isDigit(peek())) advance();
	}

	return makeToken(TOKEN_NUMBER);
}

// generate Identifier Token to variables
static Token identifier() {
	while (isAlpha(peek()) || isDigit(peek())) advance();
	return makeToken(identifierType());
}

// tells if ther is a keyword or identifier
static TokenType identifierType() {
	switch (scanner.start[0]) {
	case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
	case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
	case 'e': return checkKeyword(1, 3, "else", TOKEN_ELSE);
	case 'f':
		if (scanner.current - scanner.start > 1) {
			switch (scanner.start[1]) {
			case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
			case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
			case 'r': return checkKeyword(2, 1, "n", TOKEN_FUN);
			}
		}
		break;
	case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
	case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
	case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
	case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
	case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
	case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
	case 't': 
		if (scanner.current - scanner.start > 1) {
			switch (scanner.start[1])
			{
			case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
			case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
			}
		}
		break;
	case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
	case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
	}

	return TOKEN_IDENTIFIER;
}

// check if rest of the keyword is there
static TokenType checkKeyword(int start, int length,
							  const char* rest, TokenType type) {
	if (scanner.current - scanner.start == start + length &&
		memcmp(scanner.start + start, rest, length) == 0) {
		return type;	// return what keyword is it
	}

	return TOKEN_IDENTIFIER;
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
			printf("in /t\n");

			scanner.line++;
			advance();
			break;
		case '/':
			printf("in /\n");

			if (peekNext() == '/') {
				// comment goes until the end of line
				while (peek() != '\n' && !isAtEnd()) advance();
			}
			else return;
			break;
		default:
			break;
		}
	}
}