#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

int main() {
    FILE *file;
    char filename[100] = "3sujit.txt"; // Change this to your file name
    char ch;
    int characters = 0, words = 0, lines = 0;
    int inWord = 0;

    // Open the file
    file = fopen(filename, "r");
    if (file == NULL) {
	printf("Error opening file.\n");
	return 1;
    }

    // Read characters one by one
    while ((ch = fgetc(file)) != EOF) {
	characters++;

	if (ch == '\n') {
	    lines++;
	}

	if (isspace(ch)) {
	    inWord = 0;
	} else if (!inWord) {
	    inWord = 1;
	    words++;
	}
    }

    // Close the file
    fclose(file);

    // Display the results
    printf("Total No of Characters: %d\n", characters);
    printf("Total No of Words: %d\n", words);
    printf("Total No of Lines: %d\n", lines);

    getch();
    return 0;
}