// Implementation of the String List ADT

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

struct list {
	struct node *head;
	struct node *tail;
	int size;
};

struct node {
	char *str;
	struct node *next;
};

static struct node *newNode(char *str);
static char *myStrdup(char *str);
static int qsortStrcmp(const void *ptr1, const void *ptr2);

////////////////////////////////////////////////////////////////////////

// Creates a new empty list
List ListNew(void) {
	List l = malloc(sizeof(*l));
	if (l == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
	return l;
}

// Frees all memory allocated for the given list
void ListFree(List l) {
	struct node *curr = l->head;
	while (curr != NULL) {
		struct node *temp = curr;
		curr = curr->next;
		free(temp->str);
		free(temp);
	}
	free(l);
}

// Adds a string to the end of the list
void ListAppend(List l, char *str) {
	struct node *n = newNode(str);
	if (l->head == NULL) {
		l->head = n;
	} else {
		l->tail->next = n;
	}
	l->tail = n;
	l->size++;
}

static struct node *newNode(char *str) {
	struct node *n = malloc(sizeof(*n));
	if (n == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	n->str = myStrdup(str);
	n->next = NULL;
	return n;
}

static char *myStrdup(char *str) {
	char *copy = malloc((strlen(str) + 1) * sizeof(char));
	if (copy == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	return strcpy(copy, str);
}

// Returns the number of items in the list
int ListSize(List l) {
	return l->size;
}

// Sorts the list in ASCII order
void ListSort(List l) {
	char **items = malloc(l->size * sizeof(char *));
	if (items == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	int i = 0;
	for (struct node *curr = l->head; curr != NULL; curr = curr->next) {
		items[i++] = curr->str;
	}
	qsort(items, l->size, sizeof(char *), qsortStrcmp);
	i = 0;
	for (struct node *curr = l->head; curr != NULL; curr = curr->next) {
		curr->str = items[i++];
	}
	free(items);
}

static int qsortStrcmp(const void *ptr1, const void *ptr2) {
	char *s1 = *(char **)ptr1;
	char *s2 = *(char **)ptr2;
	return strcmp(s1, s2);
}

// Prints the list, one string per line
// If the strings themselves contain newlines, too bad
void ListPrint(List l) {
	for (struct node *n = l->head; n != NULL; n = n->next) {
		printf("%s\n", n->str);
	}
}

////////////////////////////////////////////////////////////////////////

struct listIterator {
	struct node *curr;
	List list;
};

// Creates an iterator for the given list
ListIterator ListItNew(List l) {
	ListIterator it = malloc(sizeof(*it));
	if (it == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	it->curr = l->head;
	it->list = l;
	return it;
}

// Gets the next item in the list. The item should not be modified.
char *ListItNext(ListIterator it) {
	if (it->curr == NULL) {
		fprintf(stderr, "error: no more items in iterator!\n");
		exit(EXIT_FAILURE);
	}

	char *item = it->curr->str;
	it->curr = it->curr->next;
	return item;
}

// Checks if the list has a next item
bool ListItHasNext(ListIterator it) {
	return it->curr != NULL;
}

// Frees the given iterator
void ListItFree(ListIterator it) {
	free(it);
}

