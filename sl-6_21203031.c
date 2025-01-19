#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_TOKEN_LEN 100
#define MAX_EXPR_LEN 256

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_TIMES,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
    TOKEN_INVALID
} TokenType;

typedef struct {
    TokenType type;
    int value; 
} Token;

char *input;
Token current_token;
int pos = 0;


void next_token();
int parse_expression();
int parse_term();
int parse_factor();
int evaluate_expression();

void error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

// Lexer function
void next_token() {
    while (isspace(input[pos])) pos++; // Skip whitespace

    if (isdigit(input[pos])) {
        current_token.type = TOKEN_NUMBER;
        current_token.value = 0;
        while (isdigit(input[pos])) {
            current_token.value = current_token.value * 10 + (input[pos] - '0');
            pos++;
        }

        
        if (input[pos] == '(') {
            current_token.type = TOKEN_TIMES; /
            return; 
        }
        return;
    }

    switch (input[pos]) {
        case '+':
            current_token.type = TOKEN_PLUS;
            pos++;
            break;
        case '-':
            current_token.type = TOKEN_MINUS;
            pos++;
            break;
        case '*':
            current_token.type = TOKEN_TIMES;
            pos++;
            break;
        case '/':
            current_token.type = TOKEN_DIVIDE;
            pos++;
            break;
        case '(':
            current_token.type = TOKEN_LPAREN;
            pos++;
            break;
        case ')':
            current_token.type = TOKEN_RPAREN;
            pos++;
            break;
        case '\0':
            current_token.type = TOKEN_END;
            break;
        default:
            current_token.type = TOKEN_INVALID;
            break;
    }
}

// Parser functions
int parse_expression() {
    int result = parse_term();
    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        TokenType op = current_token.type;
        next_token();
        if (op == TOKEN_PLUS) {
            result += parse_term();
        } else {
            result -= parse_term();
        }
    }
    return result;
}

int parse_term() {
    int result = parse_factor();
    while (current_token.type == TOKEN_TIMES || current_token.type == TOKEN_DIVIDE) {
        TokenType op = current_token.type;
        next_token();
        if (op == TOKEN_TIMES) {
            result *= parse_factor();
        } else {
            int divisor = parse_factor();
            if (divisor == 0) error("Division by zero");
            result /= divisor;
        }
    }
    return result;
}

int parse_factor() {
    int result;
    if (current_token.type == TOKEN_NUMBER) {
        result = current_token.value;
        next_token();
    } else if (current_token.type == TOKEN_LPAREN) {
        next_token();
        result = parse_expression();
        if (current_token.type != TOKEN_RPAREN) {
            error("Missing closing parenthesis");
        }
        next_token();
    } else {
        error("Invalid factor");
    }
    return result;
}


int evaluate_expression() {
    next_token(); 
    return parse_expression();
}

// Main function
int main() {
    char expression[MAX_EXPR_LEN];

    printf("Enter a mathematical expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = 0; // Remove newline

    input = expression;

    int result = evaluate_expression();
    printf("Valid expression. Result: %d\n", result);

    return 0;
}
