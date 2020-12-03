#ifndef _SORT_H_
#define _SORT_H_
#include "dlist.h"

//-----Function Prototypes-----//
void insertion_sort(dlist *list);
void selection_sort(dlist *list);
void merge_sort(dlist *list1, int recursion_num);
void quick_sort(dlist *list, int left, int right, int recursion_num);

#endif