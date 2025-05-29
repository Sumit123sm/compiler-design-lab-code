#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <conio.h>

#define BUFFER_SIZE 100
#define MAX_TOKEN_LEN 100

// Keywords to be recognized
const char *keywords[] = {"if", "then", "else", "while", "int", "float", "return"};
const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

// Function to check if a string is a keyword
int isKeyword(const char *str) {
    for (int i = 0; i < num_keywords; i++) {
	if (strcmp(str, keywords[i]) == 0) {
	    return 1;
	}
    }
    return 0;
}

// Function to map relational operators to their token names
const char *getRelopToken(const char *op) {
    if (strcmp(op, ">") == 0) return "GT";
    if (strcmp(op, "<") == 0) return "LT";
    if (strcmp(op, "==") == 0) return "EQ";
    if (strcmp(op, "!=") == 0) return "NE";
    if (strcmp(op, ">=") == 0) return "GE";
    if (strcmp(op, "<=") == 0) return "LE";
    return "UNKNOWN"; // Fallback for unknown operators
}

// Function to recognize tokens
void recognizeTokens(FILE *file) {
    char buffer[BUFFER_SIZE];
    char token[MAX_TOKEN_LEN];
    int tokenIndex = 0;
    int ch;
    int idCounter = 1; // Counter for identifier numbering

    // Read the input file into the buffer
    fread(buffer, 1, BUFFER_SIZE, file);

    for (int i = 0; i < BUFFER_SIZE && buffer[i] != '\0'; i++) {
	ch = buffer[i];

	if (isalpha(ch) || ch == '_') {
	    // Handle identifiers and keywords
	    tokenIndex = 0;
	    while (isalnum(ch) || ch == '_') {
		token[tokenIndex++] = ch;
		ch = buffer[++i];
	    }
	    token[tokenIndex] = '\0';
	    i--; // Unread the last character

	    if (isKeyword(token)) {
		printf("<%s> ", token); // Print keyword token
	    } else {
		printf("<ID,%d> ", idCounter++); // Print identifier token
	    }
	} else if (isdigit(ch)) {
	    // Handle numbers (integers and floats)
	    tokenIndex = 0;
	    while (isdigit(ch) || ch == '.') {
		token[tokenIndex++] = ch;
		ch = buffer[++i];
	    }
	    token[tokenIndex] = '\0';
	    i--; // Unread the last character

	    printf("<NUM,%s> ", token); // Print number token
	} else if (isspace(ch)) {
	    // Skip whitespace
	    continue;
	} else if (ch == '>' || ch == '<' || ch == '=' || ch == '!') {
	    // Handle relational operators (>, <, ==, !=, >=, <=)
	    tokenIndex = 0;
	    token[tokenIndex++] = ch;
	    ch = buffer[++i];
	    if (ch == '=') {
		token[tokenIndex++] = ch; // Handle >=, <=, ==, !=
	    } else {
		i--; // Unread the last character
	    }
	    token[tokenIndex] = '\0';
	    printf("<Relop,%s> ", getRelopToken(token)); // Print relational operator token
	} else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
	    // Handle arithmetic operators
	    printf("<ArithOp,%c> ", ch); // Print arithmetic operator token
	} else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == ';') {
	    // Handle special symbols
	    printf("<Symbol,%c> ", ch); // Print symbol token
	} else {
	    // Handle unknown characters
	    printf("<Unknown,%c> ", ch); // Print unknown token
	}
    }
}

int main() {
    FILE *file = fopen("sujit.txt", "r");
    if (file == NULL) {
	perror("Error opening file");
	return 1;
    }

    recognizeTokens(file);

    fclose(file);
    getch();
    return 0;
}