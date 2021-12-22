#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linkedList.h"

// Returns a heap ptr after checking for successful allocation
void *auto_malloc(size_t size){
    void *ptr = NULL;
    ptr = malloc(size);
    if (ptr == NULL)
    {
        printf("Malloc failed");
        exit(1);
    }
    return ptr;
}

// Releases heap space
void freeList(node* list){
   node* tmp;
   while (list != NULL)
   {
       tmp = list;
       list = list->next;
       free(tmp->program);
       free(tmp);
    }
}

node* createNode(pid_t pid, char* program){
    node* newNode = (node*)auto_malloc(sizeof(node));
    // Set data that needs to be alloacted space
    // memset used to ensure the string is null terminated
    newNode->program = (char*)auto_malloc((strlen(program) + 1) * sizeof(char));
    memset(newNode->program, '\0', strlen(program) + 1);
    strncpy(newNode->program, program, strlen(program));
    // Set data that does not have be allocated
    newNode->pid = pid;
    newNode->active = 1;
    newNode->next = NULL;
    return newNode;
}

int getListLength(node* list){
    int length = 0;
    while (list != NULL)
    {
        length += 1;
        list = list->next;
    }
    return length;
}

void printList(node* list){
    int listLength = getListLength(list);
    while (list != NULL)
    {
        printf("%d: %s\n", list->pid, list->program);
        list = list->next;
    }
    printf("Total background jobs: %d\n", listLength);
}

// Find element and returns node
// If remove flag is set, then removed the node and return it
node* findElement(node** list, pid_t wantedPid, int remove){
    node* prevNode = NULL;
    node* listNode = *list;
    while (listNode != NULL)
    {
        if(listNode->pid == wantedPid)
        {
            if(remove)
            {
                node* removedNode = listNode;
                if(prevNode != NULL)
                {
                    // Remove mid of the list
                    prevNode->next = listNode->next;
                    return removedNode;
                }
                else
                {
                    // Remove the front of the list
                    if(getListLength(*list) == 1)
                    {
                        (*list) = NULL;
                    } 
                    else
                    {
                        (*list) = listNode->next;
                    }
                    return removedNode;
                }
            }
            return listNode;     
        }
        prevNode = listNode;
        listNode = listNode->next;
    }
    return NULL;
}

// Adds node at the end of the list
void addTaskToTheList(node** list, node* newNode){
    newNode->next = NULL;
    if(*list == NULL)
    {
        *list = newNode;
        return;
    }
    node* listNode = *list;
    while (listNode->next != NULL)
    {
        listNode = listNode->next;
    }
    listNode->next = newNode;
}