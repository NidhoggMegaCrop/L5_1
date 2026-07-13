// Implementation of the Map ADT

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"

#define DEFAULT_CAPACITY 8

struct map {
	int size;
	int capacity;
	struct item *items;
};

struct item {
	char *key;
	int value;
};

static int ceilingIndex(Map m, char *key);
static void increaseCapacity(Map m);

Map MapNew(void) {
	Map m = malloc(sizeof(*m));
	if (m == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}

	m->size = 0;
	m->capacity = DEFAULT_CAPACITY;
	m->items = malloc(m->capacity * sizeof(struct item));
	if (m->items == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	return m;
}

void MapFree(Map m) {
	for (int i = 0; i < m->size; i++) {
		free(m->items[i].key);
	}
	free(m->items);
	free(m);
}

void MapSet(Map m, char *key, int value) {
	int index = ceilingIndex(m, key);

	if (index != -1 && strcmp(m->items[index].key, key) == 0) {
		m->items[index].value = value;
		return;
	}

	if (m->size == m->capacity) {
		increaseCapacity(m);
	}

	int insertIndex = index == -1 ? m->size : index;
	for (int i = m->size - 1; i >= insertIndex; i--) {
		m->items[i + 1] = m->items[i];
	}

	m->items[insertIndex] = (struct item){strdup(key), value};

	m->size++;
}

// Returns the index of the smallest key that is greater than or equal
// to the given key, or -1 if no such key exists
static int ceilingIndex(Map m, char *key) {
	int index = -1;
	int lo = 0;
	int hi = m->size - 1;

	while (lo <= hi) {
		int mid = (lo + hi) / 2;
		int cmp = strcmp(key, m->items[mid].key);

		if (cmp < 0) {
			index = mid;
			hi = mid - 1;
		} else if (cmp > 0) {
			lo = mid + 1;
		} else {
			return mid;
		}
	}

	return index;
}

static void increaseCapacity(Map m) {
	m->capacity *= 2;
	m->items = realloc(m->items, m->capacity * sizeof(struct item));
	if (m->items == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}
}

bool MapContains(Map m, char *key) {
	int index = ceilingIndex(m, key);

	return index != -1 && strcmp(m->items[index].key, key) == 0;
}

int MapGet(Map m, char *key) {
	int index = ceilingIndex(m, key);

	if (index == -1 || strcmp(m->items[index].key, key) != 0) {
		fprintf(stderr, "error: key '%s' does not exist\n", key);
		exit(EXIT_FAILURE);
	}

	return m->items[index].value;
}

