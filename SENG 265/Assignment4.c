#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
char  magic_number_1[] = { 0xBA, 0x5E, 0xBA, 0x11, 0x00 };
char  magic_number_2[] = { 0xBA, 0x5E, 0xBA, 0x12, 0x00 };
/*The encode() function may remain unchanged for A#4.*/
int encode(FILE *input, FILE *output) {
	return 0;
}
/*Amaan Makhani
Seng 265 assignment 4
date started: April 7, 2019
date due: March 27, 2019
Linked list functions ideas from Zastre dynamic memory slides
Assignment description:
This decoder uses dynamic memory to make txt file
Innovative features:
Using a linked list and dynamic memory to decode files based on size
*/

/*struct that represents a node*/
typedef struct node node;
struct node {
	char *word;
	node *next;
	int length;
};
/*function to preform malloc*/
void *auto_malloc(size_t size) {
	/*do malloc and check if null, if not return ptr*/
	void *ptr;
	ptr = malloc(size);
	if (ptr == NULL) {
		printf("malloc failed");
		exit(1);
	}
	return ptr;
}
/*if space needed print it*/
void print_space(_Bool space_needed, FILE *output) {
	if (space_needed) {
		fprintf(output, " ");
	}
}
/*function to add to front*/
node *add_front(node *head, node *new) {
	new->next = head;
	return new;
}
/*function to create a node*/
node *newnode(char *word, int length) {
	node *new_node;
	new_node = (node *)auto_malloc(sizeof(node));
	new_node->word = word;
	new_node->next = NULL;
	new_node->length = length;
	return new_node;
}
/*free list*/
void free_list(node *head) {
	node *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp->word);
		free(temp);
	}
	free(head);
}
/*add word in list*/
node *add_to(int char_count, char *characters, node *head) {
	if (char_count != 0) {
		char *word = (char*)auto_malloc(sizeof(char)*char_count);
		strncpy(word, characters, char_count);
		node *new = newnode(word, char_count);
		node* newhead = add_front(head, new);
		return newhead;
	}
	return head;
}
/*slide out to pop item*/
node *slide_out(node *head, int index, int list_length, FILE *output) {
	int count = 0;
	if (index == 0) {
		//head, print it
		for (int i = 0; i < (head->length); i++) {
			fprintf(output, "%c", head->word[i]);
		}
	}
	else if (index == list_length) {
		//last
		node *last;
		last = head;
		node *prev;
		prev = head;
		while (last->next != NULL) {
			last = last->next;
		}
		while (prev->next != last) {
			prev = prev->next;
		}
		prev->next = NULL;
		//print last
		for (int i = 0; i < (last->length); i++) {
			fprintf(output, "%c", last->word[i]);
		}
		head = add_front(head, last);
		return head;
	}
	else {
		//element in the middle
		node *elem;
		elem = head;
		while (elem != NULL) {
			elem = elem->next;
			count += 1;
			if (index == count) {
				break;
			}
		}
		node *prev;
		prev = head;
		while (prev->next != elem) {
			prev = prev->next;
		}
		for (int i = 0; i < (elem->length); i++) {
			fprintf(output, "%c", elem->word[i]);
		}
		prev->next = elem->next;
		head = add_front(head, elem);
		return head;
	}
	return head;
}
/*one char numbers*/
node *case_1(int num, int *unique, char *characters, int char_count, node *head, FILE *output, _Bool *space_needed) {
	if ((num - 128) > (*unique)) {
		*unique += 1;
		print_space(*space_needed, output);
		head = add_to(char_count, characters, head);
		*space_needed = true;
	}
	else {
		print_space(*space_needed, output);
		head = add_to(char_count, characters, head);
		int pop = (num - 129);
		head = slide_out(head, pop, *unique, output);
		*space_needed = true;
		return head;
	}
	return head;
}
/*add letter to word*/
void case_2(int chr, int count, char *characters, FILE *output) {
	characters[count] = (char)chr;
	fprintf(output, "%c", characters[count]);
}

/*decode main*/
int decode(FILE *input, FILE *output) {
	//flags and counters needed
	int chr;
	int unique = 0;
	int counter = 0;
	int char_count = 0;
	char characters[50];
	bool space_needed = false;
	bool two_char = false;
	bool three_char = false;
	int two_int = 0;
	int three_int = -1;

	//create head
	node *head;
	head = NULL;

	//read char by char and see if magic numbers contained
	while ((chr = fgetc(input)) != EOF) {
		if (counter < 4) {
			counter += 1;
			if (counter == 1) {
				if ((int)chr != 186) {
					printf("No magic numbers");
					return 1;
				}
			}
			else if (counter == 2) {
				if ((int)chr != 94) {
					printf("No magic numbers");
					return 1;
				}
			}
			else if (counter == 3) {
				if ((int)chr != 186) {
					printf("No magic numbers");
					return 1;
				}
			}
			else if (counter == 4) {
				if ((int)chr != 17 && (int)chr != 18) {
					printf("No magic numbers");
					return 1;
				}
			}
		}//magic numbers contained
		else if (two_char) {
			//two char second number
			head = add_to(char_count, characters, head);
			two_int = (int)chr + 121;
			//check if unique or exists
			if (two_int < unique) {
				print_space(space_needed, output);
				head = slide_out(head, two_int - 1, unique, output);
				space_needed = true;
			}
			else {
				print_space(space_needed, output);
				space_needed = true;
				unique += 1;
			}
			//leave case set flags and counters back
			two_char = false;
			char_count = 0;
			continue;
		}
		else if (three_char) {
			//second or third char in three char
			head = add_to(char_count, characters, head);
			if (three_int == -1) {
				//char two
				three_int = ((int)chr * 256) + 376;
			}
			else {
				//char three check if unique or exists
				three_int += (int)chr;
				if (three_int < unique) {
					print_space(space_needed, output);
					head = slide_out(head, three_int - 1, unique, output);
					space_needed = true;
				}
				else {
					print_space(space_needed, output);
					space_needed = true;
					unique += 1;
				}
				//set flags back
				three_char = false;
				three_int = -1;
			}
			char_count = 0;
			continue;
		}
		else {
			if ((int)chr == 10) {
				//newline to file, add word if needed, reset flags, and counters
				fprintf(output, "\n");
				head = add_to(char_count, characters, head);
				char_count = 0;
				space_needed = false;
			}
			else if ((int)chr < 128) {
				/*letter*/
				case_2(chr, char_count, characters, output);
				char_count += 1;
				space_needed = true;
			}
			else if (128 < (int)chr && (int)chr <= 248) {
				/*number*/
				head = case_1((int)chr, &unique, characters, char_count, head, output, &space_needed);
				char_count = 0;
			}
			else if ((int)chr == 249) {
				two_char = true;
			}
			else if ((int)chr == 250) {
				three_char = true;
			}
		}
	}
	//free list
	free_list(head);
	return 0;
}
