#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*define max size for line*/
#define LINESIZE 80
#define MAX_LINES 100
/*Amaan Makhani
Seng 265 assignment 1
date started: Jan 29, 2019
date due: Feb 12, 2019
*/

/*Assignment description:
This encoder is built to compress data for smaller transition by replacing
words with fewer codes and character.
*/

/*Assignment functions:
-reads lines in from file
-use strok_r()
-separates unique words
-create a struct to hold a word, number, and a flag for each unique struct
-create an array of structs
-with their number store both in a struct and push it into the array
-now go through text and do one of two things
>if unique word that has not been seen before replace it with number then word.
Change flag to true. push it into an array.
>if it has been seen before go to stack find its position replace it with that number.
Move word to top of the array.
-output all of this to a file with .mtf file
*/

/*Innovative features:
moving around a stack of pointers instaed of big blocks of data.
storing unique words in an array of structs.
*/

/*struct declaration*/
struct words {
	int index;
	char *letter;
	int flag;
};

/*search function for unique*/
int uniquesearch(char token[], struct words dictionary[], int current) {
	/*if it's empty return nonexistent*/
	if (dictionary[0].index != 0) {
		return 0;
	}

	int i = 0;
	while (i < current) {
		int compare = strcmp(token, dictionary[i].letter);
		if (compare == 0) {
			return 1;
		}/*if you find it return existent*/
		i++;
		/*increment index*/
	}
	return 0;
	/*return nonexistent*/
}

/*search for word in dictionary and see if its been used before*/
int search(char token[], struct words dictionary[], int current) {
	/*find word in dictionary*/
	int i = 0;
	while (i < current) {
		int compare = strcmp(token, dictionary[i].letter);
		if (compare == 0) {
			if (dictionary[i].flag == 0) {
				dictionary[i].flag = 1;
				return dictionary[i].index;
			}
			else {
				return -1;
			}
		}
		i++;
		/*increment index*/
	}
	return -2;
}

/*find a words position in a stack*/
int find_in_stack(char *stack[], int *stack_pos, char token[]) {
	int i = 0;
	while (i < *stack_pos) {
		int compare = strcmp(stack[i], token);
		if (compare == 0) {
			return *stack_pos - (i);
		}
		i += 1;
	}
	return 0;
}

/*add to stack if it has just been used*/
void add_to_stack(int find, struct words dictionary[], char *stack[], int *stack_pos) {
	stack[*stack_pos] = dictionary[find].letter;
	*stack_pos += 1;
}

/*add existing one back to front of the stack at top*/
void add_existing(char *stack[], char *temp, int *stack_pos) {
	stack[*stack_pos - 1] = temp;
}

/*remove from stack if you are using it*/
void remove_from_stack(int position, char *stack[], int *stack_pos) {
	int number = *stack_pos - position;
	char *temp = stack[number];
	for (int i = number; i < *stack_pos; i++) {
		stack[i] = stack[i + 1];
	}
	add_existing(stack, temp, stack_pos);
}

/*apply function*/
void apply(char *stack[], struct words dictionary[], int current, char line[], int *stack_pos, FILE *output) {
	/*once found prints its position in the stack,remove from the stack and place on top*/
	/*create token*/
	char delim[4] = " \n";
	char *token = strtok(line, delim);
	/*use token*/
	while (token != NULL) {
		int find = search(token, dictionary, current);

		if (find == -1) {
			/*go into stack find word*/
			int position;
			position = find_in_stack(stack, stack_pos, token);
			fputc(position + 128, output);
			/*remove from stack and add in stack*/
			remove_from_stack(position, stack, stack_pos);
		}
		else {
			fputc(find + 129, output);
			int i = 0;
			while (dictionary[find].letter[i]) {
				fputc(dictionary[find].letter[i], output);
				i++;
			}
			add_to_stack(find, dictionary, stack, stack_pos);
		}/*if it hasn't been used before print the index and word, then add to
		stack*/
		/*move onto the next token if it's not NULL*/
		token = strtok(NULL, delim);
	}
}

/*main program*/
int main(int arc, char *argv[]) {

	/*get filename from first argument and get file pointer to the file*/
	FILE *text = fopen(argv[1], "r");

	/*create output file and file name*/
	int length = strlen(argv[1]);
	char newname[20];
	strncpy(newname, argv[1], length - 4);
	strncat(newname, ".mtf", 5);
	FILE *output = fopen(newname, "w");

	/*print magic numbers*/
	fputc(186, output);
	fputc(94, output);
	fputc(186, output);
	fputc(17, output);

	/*if file doesn't exist end program*/
	if (text == NULL) {
		return 1;
	}

	/*create array of characters to hold line, create array for stack*/
	char line[LINESIZE][MAX_LINES];
	char temp[LINESIZE][MAX_LINES];
	int stack_pos = 0;
	struct words dictionary[120];
	char *stack[120];
	int unique = 0;
	/*create row counters for temp and line 2d array*/
	int j = 0;
	int a = 0;
	/*read line by line*/
	while (fgets(line[j], sizeof(line), text)) {
		/*create deliminators*/
		char delim[4] = " \n";
		/*copy into temp string to tokenize*/
		strncpy(temp[a], line[j], LINESIZE);

		char *token = strtok(temp[a], delim);
		/*create token*/
		while (token != NULL) {
			/*search for word in dictionary*/
			int exists = uniquesearch(token, dictionary, unique);
			if (exists == 0) {
				/*if it doesn't exist create struct with it*/
				dictionary[unique].flag = 0;
				dictionary[unique].index = unique;
				dictionary[unique].letter = token;
				unique += 1;

			}

			/*move onto the next token if it's not NULL*/
			token = strtok(NULL, delim);
		}
		/*apply changes to the original input and output to file*/
		apply(stack, dictionary, unique, line[j], &stack_pos, output);
		fputc('\n', output);
		/*increment row*/
		j++;
		a++;
	}
	/*close file */
	fclose(text);
	fclose(output);
	/*end of program*/
	return 0;
}
