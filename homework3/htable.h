#ifndef _HTABLE_H_
#define _HTABLE_H_
#include <stdbool.h>
#define TOMBSTONE ((int*)0x1)

//-----------Structs-----------//
typedef struct htable_t {
	int **array;
	int size;
	int fullness;
	int deleted_entries;
} htable;

//-----Function Prototypes-----//
htable *create_htable();
void delete_htable(htable *table);
int find_entry(htable *table, int entry, int *available_slot);
float load_factor(htable *table);
int ins_rehash(htable *table);
int rmv_rehash(htable *table);
int insert_entry(htable *table, int entry, bool rehashing);
int remove_entry(htable *table, int entry, bool rehashing);
void print_htable(htable *table);
#endif