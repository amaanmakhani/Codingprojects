// Data structure
typedef struct node{
    pid_t pid;
    char* program;
    int active;
    struct node* next;
} node;
// Functions
void *auto_malloc(size_t size);
void freeList(node* list);
node* createNode(pid_t pid, char* program);
int getListLength(node* list);
void printList(node* list);
node* findElement(node** list, pid_t wantedPid, int remove);
void addTaskToTheList(node** list, node* newNode);