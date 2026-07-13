// Implementation of the FriendBook ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "List.h"
#include "Map.h"
#include "Queue.h"

#define DEFAULT_CAPACITY 1 // DO NOT change this line

// DO NOT modify this struct
struct fb {
	int numPeople;
	int capacity;

	char **names; // the id of a person is simply the index
	              // that contains their name in this array

	Map nameToId; // maps names to ids

	struct adjNode **adj; // adjacency lists, kept in increasing order
};

// DO NOT modify this struct
struct adjNode {
	int v;
	struct adjNode *next;
};

static void freeAdjList(struct adjNode *list);
static void increaseCapacity(Fb fb);
static int nameToId(Fb fb, char *name);
static bool inAdjList(struct adjNode *list, int v);
static struct adjNode *newAdjNode(int v);
static struct adjNode *insertInOrder(struct adjNode *list, int v);
static struct adjNode *deleteFromList(struct adjNode *list, int v);

// Please ignore this line
static struct adjNode * __attribute__((unused)) newAdjNode(int v);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb FbNew(void) {
	Fb fb = malloc(sizeof(*fb));
	if (fb == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	fb->numPeople = 0;
	fb->capacity = DEFAULT_CAPACITY;

	fb->names = calloc(fb->capacity, sizeof(char *));
	if (fb->names == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	fb->nameToId = MapNew();

	fb->adj = calloc(fb->capacity, sizeof(struct adjNode *));
	if (fb->adj == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	return fb;
}

void FbFree(Fb fb) {
	for (int i = 0; i < fb->numPeople; i++) {
		free(fb->names[i]);
	}
	free(fb->names);

	MapFree(fb->nameToId);

	for (int i = 0; i < fb->capacity; i++) {
		freeAdjList(fb->adj[i]);
	}
	free(fb->adj);

	free(fb);
}

static void freeAdjList(struct adjNode *list) {
	struct adjNode *curr = list;
	while (curr != NULL) {
		struct adjNode *temp = curr;
		curr = curr->next;
		free(temp);
	}
}

int FbNumPeople(Fb fb) {
	return fb->numPeople;
}

bool FbAddPerson(Fb fb, char *name) {
	if (fb->numPeople == fb->capacity) {
		increaseCapacity(fb);
	}

	if (!MapContains(fb->nameToId, name)) {
		int id = fb->numPeople++;
		fb->names[id] = strdup(name);
		MapSet(fb->nameToId, name, id);
		return true;
	} else {
		return false;
	}
}

static void increaseCapacity(Fb fb) {
	int newCapacity = fb->capacity * 2;

	fb->names = realloc(fb->names, newCapacity * sizeof(char *));
	if (fb->names == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = fb->capacity; i < newCapacity; i++) {
		fb->names[i] = NULL;
	}

	fb->adj = realloc(fb->adj, newCapacity * sizeof(struct adjNode));
	if (fb->adj == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	for (int i = fb->capacity; i < newCapacity; i++) {
		fb->adj[i] = NULL;
	}

	fb->capacity = newCapacity;
}

bool FbHasPerson(Fb fb, char *name) {
	return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
	List l = ListNew();
	for (int id = 0; id < fb->numPeople; id++) {
		ListAppend(l, fb->names[id]);
	}
	return l;
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
	int id1 = nameToId(fb, name1);
	int id2 = nameToId(fb, name2);
	return inAdjList(fb->adj[id1], id2);
}

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
	if (!MapContains(fb->nameToId, name)) {
		fprintf(stderr, "error: person '%s' does not exist!\n", name);
		exit(EXIT_FAILURE);
	}
	return MapGet(fb->nameToId, name);
}

static bool inAdjList(struct adjNode *list, int v) {
	for (struct adjNode *curr = list; curr != NULL && curr->v <= v;
			curr = curr->next) {
		if (curr->v == v) {
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
// Your tasks

bool FbFriend(Fb fb, char *name1, char *name2) {
	int id1 = nameToId(fb, name1);
	int id2 = nameToId(fb, name2);

	// If they are already friends, there is nothing to do.
	if (inAdjList(fb->adj[id1], id2)) {
		return false;
	}

	// Friendship goes both ways, so add each person to the other's
	// adjacency list, keeping the lists in ascending order.
	fb->adj[id1] = insertInOrder(fb->adj[id1], id2);
	fb->adj[id2] = insertInOrder(fb->adj[id2], id1);
	return true;
}

// Inserts v into the given adjacency list so that the list remains in
// ascending order. Assumes v is not already in the list.
static struct adjNode *insertInOrder(struct adjNode *list, int v) {
	struct adjNode *newNode = newAdjNode(v);

	// Insert at the head if the list is empty or v is smaller than the
	// first element.
	if (list == NULL || v < list->v) {
		newNode->next = list;
		return newNode;
	}

	// Otherwise, find the node after which v should be inserted.
	struct adjNode *curr = list;
	while (curr->next != NULL && curr->next->v < v) {
		curr = curr->next;
	}
	newNode->next = curr->next;
	curr->next = newNode;
	return list;
}

int FbNumFriends(Fb fb, char *name) {
	int id = nameToId(fb, name);

	// The number of friends is simply the length of the person's
	// adjacency list.
	int count = 0;
	for (struct adjNode *curr = fb->adj[id]; curr != NULL; curr = curr->next) {
		count++;
	}
	return count;
}

List FbMutualFriends(Fb fb, char *name1, char *name2) {
	int id1 = nameToId(fb, name1);
	int id2 = nameToId(fb, name2);

	List l = ListNew();

	// A person is a mutual friend if they appear in both adjacency lists.
	// Scan the friends of the first person, and add anyone who is also a
	// friend of the second person.
	for (struct adjNode *curr = fb->adj[id1]; curr != NULL; curr = curr->next) {
		if (inAdjList(fb->adj[id2], curr->v)) {
			ListAppend(l, fb->names[curr->v]);
		}
	}
	return l;
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
	int id1 = nameToId(fb, name1);
	int id2 = nameToId(fb, name2);

	// If they are not friends, there is nothing to unfriend.
	if (!inAdjList(fb->adj[id1], id2)) {
		return false;
	}

	// Unfriending goes both ways, so remove each person from the other's
	// adjacency list.
	fb->adj[id1] = deleteFromList(fb->adj[id1], id2);
	fb->adj[id2] = deleteFromList(fb->adj[id2], id1);
	return true;
}

// Removes the node containing v from the given adjacency list and frees
// it, returning the (possibly new) head of the list. If v is not in the
// list, the list is returned unchanged.
static struct adjNode *deleteFromList(struct adjNode *list, int v) {
	struct adjNode *curr = list;
	struct adjNode *prev = NULL;
	while (curr != NULL && curr->v != v) {
		prev = curr;
		curr = curr->next;
	}

	if (curr == NULL) {
		return list;
	}

	if (prev == NULL) {
		list = curr->next;
	} else {
		prev->next = curr->next;
	}
	free(curr);
	return list;
}

int FbFriendRecs1(Fb fb, char *name, struct recommendation recs[]) {
	int id = nameToId(fb, name);
	int n = fb->numPeople;

	// Count how many mutual friends each person shares with the given
	// person. For every friend f of the person, each friend of f shares
	// the mutual friend f with the person.
	int *numMutual = calloc(n, sizeof(int));
	if (numMutual == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
	for (struct adjNode *f = fb->adj[id]; f != NULL; f = f->next) {
		for (struct adjNode *g = fb->adj[f->v]; g != NULL; g = g->next) {
			numMutual[g->v]++;
		}
	}

	// Build a recommendation for each person who shares at least one
	// mutual friend with the person, is not the person themselves, and is
	// not already a friend of the person.
	int numRecs = 0;
	for (int g = 0; g < n; g++) {
		if (g == id || numMutual[g] == 0 || inAdjList(fb->adj[id], g)) {
			continue;
		}
		strcpy(recs[numRecs].name, fb->names[g]);
		recs[numRecs].numMutualFriends = numMutual[g];
		numRecs++;
	}

	free(numMutual);

	// Sort the recommendations in descending order on the number of
	// mutual friends shared, using selection sort.
	for (int i = 0; i < numRecs - 1; i++) {
		int max = i;
		for (int j = i + 1; j < numRecs; j++) {
			if (recs[j].numMutualFriends > recs[max].numMutualFriends) {
				max = j;
			}
		}
		if (max != i) {
			struct recommendation tmp = recs[i];
			recs[i] = recs[max];
			recs[max] = tmp;
		}
	}

	return numRecs;
}

////////////////////////////////////////////////////////////////////////
// Optional task

List FbFriendRecs2(Fb fb, char *name) {
	// TODO
	List l = ListNew();
	return l;
}

////////////////////////////////////////////////////////////////////////

// Creates a new adjacency list node
static struct adjNode *newAdjNode(int v) {
	struct adjNode *n = malloc(sizeof(*n));
	if (n == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	n->v = v;
	n->next = NULL;
	return n;
}

