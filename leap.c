/* 
leap.c

Implementations for leap list construction and manipulation.

Skeleton written by Grady Fitzpatrick of the University of Melbourne.

Implementation completed by Biho Shin.
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "leap.h"
#include "utils.h"


struct node {
    int value;      //data
    int cur_height; //0 offset (ie. bottom level is zero height)
    node_t **next;  //array of node_t pointers with the index corresponding to the height of the leap list
    node_t **prev;  //array of node_t pointers with the index corresponding to the height of the leap list
};


struct leapList {
    /* IMPLEMENT: Fill in structure. */

    int max_height; //1 offset (max number of linked lists allowed in this leap list)
    double p;
    
    node_t **heads; //array of node_t pointers with the index corresponding to the height of the leap list
};

/*Creates an empty leap list with the given maximum height and probability*/
struct leapList *newList(int maxHeight, double p, enum problemPart part){
    /* IMPLEMENT: Set up list */
    leaplist_t *newList = (leaplist_t *)malloc(1*sizeof(leaplist_t));
    assert(newList); 

    //Insert the given values into the leap list
    newList->max_height = maxHeight;
    newList->p = p;
    newList->heads = (node_t **)malloc(maxHeight * sizeof(node_t *));

    //Initialise everything to NULL pointers
    for(int i = 0; i < maxHeight; i++) { 
        newList->heads[i] = NULL;
    }
    return newList;
}

/*Prints the given level of the list*/
void printLevel(struct leapList *list, int level){
    if(! list){
        printf("\n");
        return;
    } else {
    /* IMPLEMENT (Part B): loop over list at given level, printing out each value. */
    /* Note: while additional next elements, print a space after the key. If no additional next elements, 
    print a new line and then return. */
        node_t *traverser = list->heads[level];
        if (traverser == NULL) {
            printf("\n");
            return;
        }
        while (traverser) {
            int value = traverser->value;
            traverser = traverser->next[level];
            if (traverser != NULL) {  //not the last element in that level of leap list
                printf("%d ", value);
            } else {
                printf("%d\n", value); //is the last element in that level of leap list
            }
        }
    }
    return;
}

/*Creates a new node for respective leap list and initialises with NULL pointers, the given key value, and zero for height */
node_t *newNode(int key, leaplist_t *list) {
    node_t *new = (node_t *)malloc(1*sizeof(node_t));
    assert(new);
    new->cur_height = 0;
    new->value = key;
    new->next = (node_t **)malloc(list->max_height * sizeof(node_t *));
    new->prev = (node_t **)malloc(list->max_height * sizeof(node_t *));
    for (int i = 0; i < list->max_height; i++) {
        (new->next)[i] = (new->prev)[i] = NULL;
    }
    return new;
}

/*Adds a new key to the list*/
void insertKey(int key, struct leapList *list){
    /* IMPLEMENT: Insert the key into the given leap list. */
    node_t *new = newNode(key, list);

    int lvl = list->max_height - 1;
    int inserted = 0; //Flag variable to check if we have found a location for the new value in the base level of leap list
    node_t *traverser = (list->heads)[lvl];

    while (lvl >= 0 && inserted == 0) {
        //If the level of the leap list doesn't exist and we're not at the bottom lvl, skip it and go down
        if ((list->heads)[lvl] == NULL) { 
            if (lvl > 0) { 
                lvl--;
                traverser = (list->heads)[lvl];
                continue;
            } else { //Nothing has been read into the leap list yet and we are at bottom lvl
                (list->heads)[lvl] = new;
                inserted = 1; //New node inserted successfully
                break;
            }
        }

        //Traverse through that level until you find the maximal element that is still smaller than k
        while (traverser->value <= key) {

            if ((traverser->next)[lvl] != NULL) { //If we're not at the end of the linked list for that level
                traverser = (traverser->next)[lvl];

            } else { //Traverser has reached the end of that level in the leap list and its value is smaller than k
                if (lvl == 0) { //If at bottom level, insert new element here
                    (traverser->next)[lvl] = new;
                    (new->prev)[lvl] = traverser;
                    inserted = 1; //New node inserted successfully
                    break;
                } else { //Move down a level and continue searching
                    lvl--;
                    continue;
                }
            }
        }

        //We arrive here if traverser is at a node with value > k
        if (inserted == 0) {

            if (lvl > 0) {//If not at bottom level, backtrack(if possible) and drop down and continue searching
                if (traverser == (list->heads)[lvl]) { //backtrack is not possible (at head of current lvl), go to head of lower level
                    lvl--;
                    traverser = (list->heads)[lvl];
                } else { //backtrack is possible
                    traverser = (traverser->prev)[lvl];
                    lvl--;
                }
                continue;

            } else { //If at bottom lvl, insert
                node_t *before = (traverser->prev)[lvl];
                node_t *after = traverser;
                if (before == NULL) { //Backtrack not possible
                    (list->heads)[lvl] = new;
                    (new->next)[lvl] = after;
                    (after->prev)[lvl] = new;
                } else { //Backtrack possible
                    (before->next)[lvl] = new;
                    (new->prev)[lvl] = before;
                    (after->prev)[lvl] = new;
                    (new->next)[lvl] = after;
                }
                inserted = 1; //New node inserted successfully
                break;
            }
        }
    }
    //Now we build the leap list up from bottom level using random numbers
    while ((new->cur_height < (list->max_height - 1) && inserted) && ((double) rand() / RAND_MAX < list->p)) { //As long as we are below max height
        node_t *before = taller_left_node(new);
        int new_height = new->cur_height + 1;
        new->cur_height = new_height;

        //Logic for insertion in upper levels
        if (before == NULL) {//If new inserted node is the head node for the level above
            if (list->heads[new_height] == NULL) { //If we create a new level
                list->heads[new_height] = new;
            } else { //If the level already exists and we put new node into leftmost position
                list->heads[new_height]->prev[new_height] = new;
                new->next[new_height] = list->heads[new_height];
                list->heads[new_height] = new;
            }
        } else { //Sandwich the new node in the new level between existing nodes
            node_t *after = before->next[new_height];
            (before->next)[new_height] = new;
            (new->prev)[new_height] = before;
            if (after != NULL) {
                (new->next)[new_height] = after;
                (after->prev)[new_height] = new;
            }
        }
    }
}

/*Prints out a given leap list*/
int print_leaplist(leaplist_t *list) {
    for (int level = (list->max_height) - 1; level >= 0; level--) {
        node_t *traverser = (list->heads)[level];
        if (traverser == NULL) {
            printf("NULL\n");
            continue;
        } else {
            while (traverser != NULL) {
                printf("%d ", traverser->value);
                //Increment traverser
                traverser = (traverser->next)[level];
            }
        }
        printf("\n");
    }
    return 0;
}

/* Finds the location of the nearest node that is to the left and is taller than a given node 
or returns NULL if such a node does not exist for a given leap list.*/
node_t *taller_left_node(node_t *start_node) {
    node_t *traverser = start_node;
    int height = start_node->cur_height;
    //If the given node is already the leftmost node for its current level return NULL
    if ((traverser->prev)[height] == NULL) {
        return NULL;
    }
    //Keep going back until you find another node that also occupies a higher level than the one you're currently at
    while (traverser->cur_height <= height) {
        //If we're sitting on the leftmost node then return NULL
        if (traverser->prev[height] == NULL) {
            return NULL;
        }
        traverser = traverser->prev[height];
    }
    //Loop terminates when we first encounter a viable node
    return traverser;
}

/* Queries the leap list for the given key and places the result in the solution structure. */
int findKey(int key, struct leapList *list, enum problemPart part, struct solution *solution){
    int found = NOTFOUND;
    int element = key;
    int baseAccesses = base_access_finder(key, list);
    int requiredAccesses = 0;
    assert(solution);
    /* IMPLEMENT: Find the given key in the leap list. */

    int completed = 0; //Flag variable to determine if search process has terminated
    node_t *prev_comp_node = NULL;
    int lvl = list->max_height - 1; //Keep track of the current level of the leap list that we are stepping through
    node_t *traverser = (list->heads)[lvl];

    while (lvl >= 0 && completed == 0) {
        //If the level of the leap list doesn't exist and we're not at the bottom lvl, skip it and go down
        if ((list->heads)[lvl] == NULL) {
            if (lvl > 0) { //Not at the bottom yet
                lvl--;
                traverser = (list->heads)[lvl];
                continue;
            } else { //Leap list is empty so not found
                completed = 1;
                break;
            }
        }

        //Traverse through that level until you find the maximal element that is still smaller than k
        while (traverser->value <= key) {

            if (traverser->value == key) {
                found = 1;
                completed = 1;
                requiredAccesses++; //we have HAD a comparison
                break;
            } else if ((traverser->next)[lvl] != NULL) { //If we're not at the end of the linked list for that level
                requiredAccesses++; //we have HAD a comparison
                traverser = (traverser->next)[lvl]; //SCAN FORWARD
                
            } else { //Traverser has reached the end of the that level in the leap list and its value is smaller than k
                requiredAccesses++;
                if (lvl > 0) { 
                    //When we drop down the while loop will increment requiredAccesses so we subtract here
                    requiredAccesses--;
                    lvl--;
                    continue;
                } else { //We have reached end of bottom level - key is larger than everything in the leap list
                    completed = 1;
                    break;
                }
            }

        }
        //We get here if the traverser is at a node with value > key
        if (found == NOTFOUND && completed == 0) {
            requiredAccesses++;
            prev_comp_node = traverser; //Take note of the current node

            //If not at bottom level, backtrack (if possible) and drop down and continue searching.
            //Keep dropping down until the next node is different from the prev_comp_node or until the ground level is reached
            if (lvl > 0) {
                if (traverser == (list->heads)[lvl]) { 
                //Backtrack is not possible (ie. at head), go to head of lower level and 
                //keep going down until we find the first head node that is different from what was above or the bottom is reached
                    while (traverser == (list->heads)[lvl - 1] && lvl > 0) {
                    lvl--;
                    traverser = (list->heads)[lvl];
                        if (lvl == 0) { //If we get to the bottom then the key doesn't exist in the leap list
                            completed = 1;
                            break;
                        }
                    }
                    if (traverser != (list->heads)[lvl - 1]){
                        lvl--;
                        traverser = (list->heads)[lvl];
                    }
                } else { //Backtrack is possible
                    traverser = (traverser->prev)[lvl]; //Move back on same level
                    lvl--; //Drop down 1 level

                    //If the next node after dropping down is same as before then drop down again until different or we reach bottom level
                    while ((traverser->next)[lvl] == prev_comp_node && lvl >= 0) { 
                        lvl--; //Drop down
                        if (lvl == -1) { //If we reach bottom then not found 
                            completed = 1;
                            break;
                        }
                    }
                    traverser = traverser->next[lvl];
                }
                continue;
            } else { //If at bottom level and encounter element larger than key without having found key - NOT FOUND
                completed = 1;
                break;
            }
        }
    }
    

    /* Insert result into solution. */
    (solution->queries)++;
    solution->queryResults = (int *) realloc(solution->queryResults, sizeof(int) * solution->queries);
    assert(solution->queryResults);
    (solution->queryResults)[solution->queries - 1] = found;
    solution->queryElements = (int *) realloc(solution->queryElements, sizeof(int) * solution->queries);
    assert(solution->queryElements);
    solution->queryElements[solution->queries - 1] = element;
    solution->baseAccesses = (int *) realloc(solution->baseAccesses, sizeof(int) * solution->queries);
    assert(solution->baseAccesses);
    solution->baseAccesses[solution->queries - 1] = baseAccesses;
    solution->requiredAccesses = (int *) realloc(solution->requiredAccesses, sizeof(int) * solution->queries);
    assert(solution->requiredAccesses);
    solution->requiredAccesses[solution->queries - 1] = requiredAccesses;
    return found;
}

/*Performs a linear search in the base level of a leap list to determine the no of base accesses needed for a key to determine presence in a leap list. */
int base_access_finder(int key, leaplist_t *list) {
    node_t *traverser = list->heads[0];
    int base_accesses = 1;

    //Traverse forward until we see an element that is greater than the key or the end of the list
    while (traverser != NULL) {
        if (traverser->value == key) {
            return base_accesses;

        } else if (traverser->value < key) { //Increment the traverser
            traverser = traverser->next[0];
            if (traverser == NULL) { //We've hit the end so do not increment
                return base_accesses;
            } 
            base_accesses++;

        } else { //Found element greater than key
            return base_accesses;
        }
    }
    //We never actually hit this return statement but it's here to suppress warning
    return base_accesses;
}

/* Deletes a key from the leap list*/
//Can assume that the key will be in the leap list
void deleteKey(int key, struct leapList *list, enum problemPart part){
    /* IMPLEMENT: Remove the given key from the leap list. */

    //Find the key in leap list
    node_t *traverser = list->heads[0];
    while (traverser->value != key) {
        traverser = traverser->next[0];
    }

    //Link everything back up wrt the node that we will delete
    for (int lvl = 0; lvl <= traverser->cur_height; lvl++) {
        //For each height of the deleted node link the prev with next
        if (traverser->prev[lvl] != NULL) {
            traverser->prev[lvl]->next[lvl] = traverser->next[lvl];
        }
        if (traverser->next[lvl] != NULL) {
            traverser->next[lvl]->prev[lvl] = traverser->prev[lvl];
        }
        //Check if any of the nodes were head nodes for the leap list
        if (list->heads[lvl]->value == traverser->value) {
            list->heads[lvl] = traverser->next[lvl];
        }
    }
    //Free the node
    freeNode(traverser);
}

/* Frees all memory associated with the leap list*/
void freeList(struct leapList *list) {
    /* IMPLEMENT: Free all memory used by the list. */

    node_t *new;
    while (list->heads[0]) {
        new = list->heads[0];
        list->heads[0] = list->heads[0]->next[0];

        freeNode(new);
    }

    //Free the leap list struct itself
    free(list->heads);
    free(list);
    list = NULL;
}

/* Frees all memory associated with a leap list node*/
void freeNode(node_t *node) {
    free(node->prev);
    free(node->next);
    free(node);
    node = NULL;
} 

/* Frees all memory associated with the solution.*/
void freeSolution(struct solution *solution){
    if(! solution){
        return;
    }
    freeList(solution->list);
    if(solution->queries > 0){
        free(solution->queryResults);
        free(solution->queryElements);
        free(solution->baseAccesses);
        free(solution->requiredAccesses);
    }
    free(solution);
}


