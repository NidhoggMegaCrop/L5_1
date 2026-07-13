// Interface to the String List ADT

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct list *List;

/**
 * Creates a new empty list
 * Time complexity: O(1)
 */
List ListNew(void);

/**
 * Frees all memory allocated to the given list
 * Time complexity: O(n)
 */
void ListFree(List l);

/**
 * Adds a copy of the given string to the end of the list
 * Time complexity: O(1)
 */
void ListAppend(List l, char *s);

/**
 * Returns the number of items in the list
 * Time complexity: O(1)
 */
int ListSize(List l);

/**
 * Sorts the list in ASCII order
 * Time complexity: O(n log n)
 */
void ListSort(List l);

/**
 * Prints the list to stdout, one string per line
 * If the strings themselves contain newlines, too bad
 * Time complexity: O(n)
 */
void ListPrint(List l);

////////////////////////////////////////////////////////////////////////
// Do NOT use these functions

typedef struct listIterator *ListIterator;

/**
 * Creates an iterator for the given list
 * Time complexity: O(1)
 */
ListIterator ListItNew(List l);

/**
 * Gets the next item in the list. The item should not be modified.
 * Time complexity: O(1)
 */
char *ListItNext(ListIterator it);

/**
 * Checks if the list has a next item
 * Time complexity: O(1)
 */
bool ListItHasNext(ListIterator it);

/**
 * Frees the given iterator
 * Time complexity: O(1)
 */
void ListItFree(ListIterator it);

#endif

