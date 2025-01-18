#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token Types
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_INVALID
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    int value; // Used only for numbers
} Token;

// Global Variables
char *input;
Token currentToken;

// Function Prototypes
void nextToken();
int parseExpression();
int parseTerm();
int parseFactor();
void error(const char *message);

// Lexer: Fetch the next token
void nextToken() {
    while (isspace(*input)) input++; // Skip whitespaces

    if (*input == '\0') {
        currentToken.type = TOKEN_END;
        return;
    }

    if (isdigit(*input)) {
        currentToken.type = TOKEN_NUMBER;
        currentToken.value = 0;
        while (isdigit(*input)) {
            currentToken.value = currentToken.value * 10 + (*input - '0');
            input++;
        }
        return;
    }

    switch (*input) {
        case '+': currentToken.type = TOKEN_PLUS; input++; break;
        case '-': currentToken.type = TOKEN_MINUS; input++; break;
        case '*': currentToken.type = TOKEN_MULTIPLY; input++; break;
        case '/': currentToken.type = TOKEN_DIVIDE; input++; break;
        case '(': currentToken.type = TOKEN_LPAREN; input++; break;
        case ')': currentToken.type = TOKEN_RPAREN; input++; break;
        default:
            currentToken.type = TOKEN_INVALID;
            input++;
            break;
    }
}

// Parser Error Handling
void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Recursive Descent Parsing Functions
int parseExpression() {
    int result = parseTerm(); // Parse a term first

    // Handle addition and subtraction
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType operator = currentToken.type;
        nextToken();
        int rhs = parseTerm();
        if (operator == TOKEN_PLUS) {
            result += rhs;
        } else {
            result -= rhs;
        }
    }

    return result;
}

int parseTerm() {
    int result = parseFactor(); // Parse a factor first

    // Handle multiplication and division
    while (currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE) {
        TokenType operator = currentToken.type;
        nextToken();
        int rhs = parseFactor();
        if (operator == TOKEN_MULTIPLY) {
            result *= rhs;
        } else {
            if (rhs == 0) {
                error("Division by zero");
            }
            result /= rhs;
        }
    }

    return result;
}

int parseFactor() {
    if (currentToken.type == TOKEN_NUMBER) {
        int value = currentToken.value;
        nextToken();
        return value;
    } else if (currentToken.type == TOKEN_LPAREN) {
        nextToken();
        int result = parseExpression();
        if (currentToken.type != TOKEN_RPAREN) {
            error("Expected closing parenthesis");
        }
        nextToken();
        return result;
    } else {
        error("Invalid factor");
        return 0; // Unreachable
    }
}

// Main Function
int main() {
    char buffer[256];

    printf("Enter a mathematical expression: ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        error("Failed to read input");
    }

    // Remove trailing newline, if present
    buffer[strcspn(buffer, "\n")] = '\0';

    input = buffer;
    nextToken();

    int result = parseExpression();

    if (currentToken.type != TOKEN_END) {
        error("Unexpected characters at end of input");
    }

    printf("Result: %d\n", result);
    return 0;
}
