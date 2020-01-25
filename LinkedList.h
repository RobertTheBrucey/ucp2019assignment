/*****************
 * File: LinkedList.h
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H 1

/* Structs */
typedef struct ListNode {
    struct ListNode *next, *prev;
    void* data;
} ListNode;

typedef struct LinkedList {
    struct ListNode *head, *tail;
    int numNodes;
} LinkedList;

/* Function Prototypes */

LinkedList* createLinkedList();

void insertStart( LinkedList* list, void* inData );
void insertLast( LinkedList* list, void* inData );
void* removeStart( LinkedList* list );
void* removeLast( LinkedList* list );
void printLinkedList( LinkedList* list, void (*func)(void* data) );
void freeLinkedList( LinkedList* list, void (*func)(void* data) );
void* getNode( LinkedList* list, int index );

#endif