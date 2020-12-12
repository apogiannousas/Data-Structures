#ifndef _SORT_H_
#define _SORT_H_
#include "dlist.h"
//--------Masks for radix_sortMSD--------//
#define MSD_MASK1 0x80000000
#define MSD_MASK2 0xC0000000
#define MSD_MASK3 0xF0000000
#define MSD_MASK4 0xFF000000
#define MSD_MASK5 0xFFFF0000
//--------Masks for radix_sortLSD--------//
#define LSD_MASK1 0x00000001
#define LSD_MASK2 0x00000003
#define LSD_MASK3 0x0000000F
#define LSD_MASK4 0x000000FF
#define LSD_MASK5 0x0000FFFF

//----------------Structs----------------//
typedef struct alphabet_t {
    unsigned int digit_num;
    unsigned int digit_length;
    unsigned int mask;
} alphabet_info;

//----------Function Prototypes----------//
void insertion_sort(dlist *list);
void selection_sort(dlist *list);
void merge_sort(dlist *list1, unsigned int recursion_num);
void quick_sort(dlist *list, int left, int right, unsigned int recursion_num);
void radix_sortMSD(dlist *list, alphabet_info alphabet, unsigned int recursion_num);
void radix_sortLSD(dlist *list, alphabet_info alphabet);

#endif