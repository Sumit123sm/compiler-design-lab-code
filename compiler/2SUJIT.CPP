#include <stdio.h>
#include<conio.h>
#include <ctype.h>
#include <string.h>

#define MAX_IDENTIFIER_LENGTH 100


// List of C keywords
const char *keywords[] = {
	"auto", "break", "case", "char", "const", "continue", "default", "do", "double",
	"else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register",
	"return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
	"union", "unsigned", "void", "volatile", "while"
};

// Function to check if a string is a C keyword
int isKeyword(const char *str) {
	int n = sizeof(keywords) / sizeof(keywords[0]);
	for (int i = 0; i < n; i++) {
		if (strcmp(str, keywords[i]) == 0) {
			return 1; // It is a keyword
		}
	}
	return 0; // Not a keyword
}

// Function to check if a given string is a valid identifier
int isValidIdentifier(const char *str) {
	if (!isalpha(str[0]) && str[0] != '_') {
		return 0; // First character must be a letter or underscore
	}
	for (int i = 1; str[i] != '\0'; i++) {
		if (!isalnum(str[i]) && str[i] != '_') {
			return 0; // Only letters, digits, and underscores are allowed
		}
	}
	if (isKeyword(str)) {
		return 0; // Identifiers cannot be keywords
	}
	return 1; // Valid identifier
}

int main() {
	FILE *file = fopen("ram1.txt", "r");
	clrscr();
	if (file == NULL) {
		printf("Error opening file\n");
		return 1;
	}

	char word[MAX_IDENTIFIER_LENGTH];


	while (fscanf(file, "%s", word) != EOF) {

		if(strcmp(word,"int")==0 || strcmp(word,"float")==0 || strcmp(word,"char")==0 || strcmp(word,"main()")==0 || strcmp(word,"{")==0 || strcmp(word,"}")==0){
		continue;
		}
		int len = strlen(word);
		if(word[len-1]==','||word[len-1]==';')
			word[len-1]='\0';

		if(isValidIdentifier(word)) {
			printf("\"%s\" is a valid identifier.\n", word);
		} else {
			printf("\"%s\" is not a valid identifier.\n",word);
		}
	}

	fclose(file);
	getch();
	return 0;
	}