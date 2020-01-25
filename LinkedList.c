/*
 * File: LinkedList.c
 */
#include "LinkedList.h"
#include <stdlib.h>

LinkedList* createLinkedList()
{
    LinkedList *list = (LinkedList*)malloc( sizeof( LinkedList ) );
    list->head = NULL;
    list->tail = NULL;
    list->numNodes = 0;
    return list;
}

void insertStart( LinkedList* list, void* inData )
{
    ListNode *newNode = (ListNode*)malloc( sizeof( ListNode ) );

    newNode->data = inData;
    newNode->next = list->head;
    newNode->prev = NULL;

    list->head = newNode;
    list->numNodes++;

    if (list->tail == NULL) { list->tail = newNode; }
}

void insertLast( LinkedList* list, void* inData )
{
    ListNode *newNode = (ListNode*)malloc( sizeof( ListNode ) );

    newNode->data = inData;
    newNode->next = NULL;
    newNode->prev = list->tail;

    
    list->tail = newNode;
    list->numNodes++;

    if (list->head == NULL) { list->head = newNode; }
    else { list->tail->prev->next = newNode; }
}

void* removeStart( LinkedList* list )
{
    void* data = NULL;
    ListNode *temp = NULL;

    if (list->numNodes)
    {
        data = list->head->data;
        temp = list->head;

        list->head = temp->next;
        if (list->head != NULL)
        { list->head->prev = NULL; }
        else
        { list->tail = NULL; }

        free(temp);

        list->numNodes--;
    }

    return data;
}

void* removeLast( LinkedList* list )
{
    void* data = NULL;
    ListNode *temp;
    if (list->numNodes)
    {
        data = list->tail->data;
        temp = list->tail;

        list->tail = temp->prev;
        if (list->tail != NULL)
        { list->tail->next = NULL; }
        else
        { list->head = NULL; }

        free(temp);

        list->numNodes--;
    }

    return data;
}

void printLinkedList( LinkedList* list, void func(void* data) )
{
    ListNode *node = list->head;
    while (node != NULL)
    {
        func(node->data);
        node = node->next;
    }
}

void freeLinkedList( LinkedList* list, void func(void* data) )
{
    ListNode *temp;
    ListNode *node = list->head;
    while (node != NULL)
    {
        func(node->data);
        temp = node;
        node = node->next;
        free(temp);
        list->numNodes--;
    }
    free(list);
}

void* getNode( LinkedList* list, int index )
{
    ListNode *node = list->head;
    int ii = 0;
    while ( ii < index && node != NULL )
    {
        node = node->next;
        ii++;
    }
    return node->data;
}
