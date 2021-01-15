#include <stdio.h>
#include <stdlib.h>
#include "htable.h"

// *** create_htable *** //
htable *create_htable() {
	htable *new_htable;

	// Allocate space for a new hash table
	new_htable = (htable *) malloc(sizeof(htable));
	if (new_htable == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// Allocate space for the array of pointers to integers.
	// Always give an initial size of two
	new_htable->array = (int **) malloc(2*sizeof(int *));
	if (new_htable->array == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	new_htable->array[0] = NULL;
	new_htable->array[1] = NULL;
	new_htable->fullness = 0;
	new_htable->deleted_entries = 0;
	new_htable->size = 2;

	return new_htable;
}

// *** delete_htable *** //
void delete_htable(htable *table) {
	int i;

	for (i = 0; i < table->size; i++) {
		// Delete every slot that is not empty or has not been emptied
		if (table->array[i] != NULL && table->array[i] != TOMBSTONE) {
			free(table->array[i]);
		}
	}
	free(table->array);
	free(table);
}

// *** find_entry *** //
int find_entry(htable *table, int entry, int *available_slot) {
	int i, j, key;

	// Store expected key of this entry
	key = entry % table->size;
	*available_slot = -1;
	i = key;
	j = 1;
	// Search for entry with that key and apply linear probing until
	// either it is found or is not found or an empty slot is found 
	do {
		if (i >= table->size) {
			fprintf(stderr, "Error in find_entry: key exceeded htable size\n");
			exit(EXIT_FAILURE);
		}
		else if (table->array[i] == NULL) {
			// Store the first available slot for the entry
			if (*available_slot == -1) {
				*available_slot = i;
			}
			return 0;
		}
		else if(table->array[i] == TOMBSTONE) {
			// Store the first available slot for the entry
			if (*available_slot == -1) {
				*available_slot = i;
			}
		}
		else if (*(table->array[i]) == entry) {
			// If the entry is found store its key
			*available_slot = i;
			return 1;
		}
		// Select next possible key
		i = (key + j) % table->size;
		j++;
	} while (i != key);

	return 0;
}

// *** load_factor *** //
float load_factor(htable *table) {
	return ((float) table->fullness / table->size);
}

// *** ins_rehash *** //
int ins_rehash(htable *table) {
	int **old_array, **new_array;
	int i, old_size;
	
	if (load_factor(table) >= 0.5) {
		// Double the hash table size, clear
		// fullness and deleted_entries
		old_size = table->size;
		table->size *= 2;
		table->fullness = 0;
		table->deleted_entries = 0;
		
		// Create an array of double size
		new_array= (int **) malloc((table->size)*sizeof(int *));
		if (new_array == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		
		// Initialise new array
		old_array = table->array;
		table->array = new_array;
		for (i = 0; i < table->size; i++) {
			table->array[i] = NULL;
		}

		// Re-insert all elements into the new array
		for (i = 0; i < old_size; i++) {
			if (old_array[i] != NULL && old_array[i] != TOMBSTONE) {
				insert_entry(table, *(old_array[i]), false);
				free(old_array[i]);
			}
		}
		free(old_array);
	}
	else {
		return 0;
	}

	return 1;
}

// *** rmv_rehash *** //
int rmv_rehash(htable *table) {
	int **old_array, **new_array;
	int i, old_size;
	
	if (load_factor(table) <= 0.125) {
		// Reduce hash table size to half of its initial
		// value, clear fullness and deleted_entries
		old_size = table->size;
		table->size /= 2;
		table->fullness = 0;
		table->deleted_entries = 0;
		
		// Create an array of half size
		new_array= (int **) malloc((table->size)*sizeof(int *));
		if (new_array == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		
		// Initialise new array
		old_array = table->array;
		table->array = new_array;
		for (i = 0; i < table->size; i++) {
			table->array[i] = NULL;
		}

		// Re-insert all elements into the new array
		for (i = 0; i < old_size; i++) {
			if (old_array[i] != NULL && old_array[i] != TOMBSTONE) {
				insert_entry(table, *(old_array[i]), false);
				free(old_array[i]);
			}
		}
		free(old_array);
	}
	else {
		return 0;
	}

	return 1;
}

// *** insert_entry *** //
int insert_entry(htable *table, int entry, bool rehashing) {
	int key;

	// Apply rehashing if necessary (only if this service is on)
	if (rehashing == true) {
		ins_rehash(table);
	}

	// Check if the entry already exists
	if (find_entry(table, entry, &key) == 1) {
		return 0;
	}

	// Reduce number of deleted elements
	if (table->array[key] == TOMBSTONE) {
		table->deleted_entries--;
	}

	// Insert the entry in the hash table
	table->array[key] = (int *) malloc(sizeof(int));
	if (table->array[key] == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	*(table->array[key]) = entry;
	table->fullness++;

	return 1;
}

// *** remove_entry *** //
int remove_entry(htable *table, int entry, bool rehashing) {
	int key;

	// Check if the entry does not exist
	if (find_entry(table, entry, &key) == 0) {
		return 0;
	}

	// Remove the entry from the hash table
	free(table->array[key]);
	table->array[key] = TOMBSTONE;
	table->fullness--;
	table->deleted_entries++;

	// Apply rehashing if necessary (only if this service is on)
	if (rehashing == true && table->size != 2) {
		rmv_rehash(table);
	}

	return 1;
}

// *** print_htable *** //
void print_htable(htable *table) {
	int i;

	printf("\nSIZE: %d, INSERTED: %d, DELETED: %d\n", table->size, table->fullness, table->deleted_entries);
	for (i = 0; i < table->size; i++) {
		printf("%3d ", i);
	}

	putchar('\n');
	for (i = 0; i < table->size; i++) {
		if (table->array[i] == NULL) {
			printf("  * ");
		}
		else if (table->array[i] == TOMBSTONE) {
			printf("  # ");
		}
		else {
			printf("%3d ", *(table->array[i]));
		}
	}
	putchar('\n');
}

