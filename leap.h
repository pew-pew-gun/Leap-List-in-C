/* 
leap.h

Visible structs and functions for leap list construction and manipulation.

Skeleton written by Grady Fitzaptrick of the University of Melbourne.

Implementation completed by Biho Shin.
*/
typedef struct leapList leaplist_t;
typedef struct node node_t;
struct leapList;

enum problemPart;

/* A particular solution to an leapList problem. */
#ifndef SOLUTION_STRUCT
#define SOLUTION_STRUCT
struct solution {
    int queries;
    int *queryResults;
    int *queryElements;
    int *baseAccesses;
    int *requiredAccesses;
    /* Required for part B */
    struct leapList *list;
};
#endif

/* Which part the program should find a solution for. */
#ifndef PART_ENUM
#define PART_ENUM
enum problemPart {
    PART_A=0,
    PART_B=1
};
#endif

/* Value indicating the item is not found. */
#define NOTFOUND (-1)

/* Creates an empty leap list with the given max height and probability. */
struct leapList *newList(int maxHeight, double p, enum problemPart part);

/* Prints the given level of the list. */
void printLevel(struct leapList *list, int level);

/* Adds a new key to the leap list. */
void insertKey(int key, struct leapList *list);

/* Queries the leap list for the given key and places the result in the solution structure. */
int findKey(int key, struct leapList *list, enum problemPart part, struct solution *solution);

/* Deletes a value from the leap list. */
void deleteKey(int key, struct leapList *list, enum problemPart part);

/* Frees all memory associated with the leap list. */
void freeList(struct leapList *list);

/* Frees all memory associated with the solution. */
void freeSolution(struct solution *solution);

/*Creates a new node for respective leap list and initialises with NULL pointers, the given key value, and zero for height */
node_t *newNode(int key, leaplist_t *list);

/*Prints out a given leap list*/
int print_leaplist(leaplist_t *list);

/* Finds the location of the nearest node that is to the left and is taller than a given node 
or returns NULL if such a node does not exist for a given leap list.*/
node_t *taller_left_node(node_t *start_node);

/*Performs a linear search in the base level of a leap list to determine the no of base accesses needed for a key to determine presence in a leap list. */
int base_access_finder(int key, leaplist_t *list);

/* Frees all memory associated with a leap list node*/
void freeNode(node_t *node);

