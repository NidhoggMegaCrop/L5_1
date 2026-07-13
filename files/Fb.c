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
	// TODO: Task 1 - Implement this function
	return false;
}

int FbNumFriends(Fb fb, char *name) {
	// TODO: Task 2 - Implement this function
	return -1;
}

List FbMutualFriends(Fb fb, char *name1, char *name2) {
	// TODO: Task 3 - Implement this function
	List l = ListNew();
	return l;
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
	// TODO: Task 4 - Implement this function
	return false;
}

int FbFriendRecs1(Fb fb, char *name, struct recommendation recs[]) {
	// TODO: Task 5 - Implement this function
	return 0;
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

