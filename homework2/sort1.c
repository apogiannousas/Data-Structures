#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sort.h"

//-----Function Prototypes-----//
alphabet_info *create_alphabet(int radixsort_type, unsigned int alphabet_digit_length);

//------------Main-------------//
int main(int argc, char *argv[]) {
    enum sorting_algorithm {insort = 1, selsort, quicksort, mergesort, radixsortMSD, radixsortLSD};
    enum sorting_algorithm sort;
	alphabet_info *alphabet;
    dlist *list;

    if (argc < 2 || argc > 3) {
        printf("Invalid argument\n");
        return 1;
    }

    sort = atoi(argv[1]); 
    switch (sort) {
        case insort: {
            list = read_list();
            print_list(*list);
            putchar('\n');
            insertion_sort(list);
            break;
        }
        case selsort: {
            list = read_list();
            print_list(*list);
            putchar('\n');
            selection_sort(list);
            break;
        }
        case quicksort: {
            list = read_list();
            print_list(*list);
            putchar('\n');
            quick_sort(list, 0, list->size - 1, 0);
            break;
        }
        case mergesort: {
            list = read_list();
            print_list(*list);
            putchar('\n');
            merge_sort(list, 0);
            break;
        }
        case radixsortMSD: {
            if (argc != 3) {
				printf("Invalid argument\n");
				return 1;
			}
            alphabet = create_alphabet(radixsortMSD, (unsigned int)atoi(argv[2]));

            list = read_list();
            print_list(*list);
            putchar('\n');
			radix_sortMSD(list, *alphabet, 0);
            free(alphabet);
			break;
        }
        case radixsortLSD: {
            if (argc != 3) {
				printf("Invalid argument\n");
				return 1;
			}
            alphabet = create_alphabet(radixsortLSD, (unsigned int)atoi(argv[2]));

            list = read_list();
            print_list(*list);
            putchar('\n');
			radix_sortLSD(list, *alphabet);
            free(alphabet);
			break;
        }
        default: {
            printf("Invalid argument\n");
            return 1;
        }
    }

    putchar('\n');
    print_list(*list);
    delete_list(list);

    return 0;
}

//---Funtion Implementations---//

// *** create_alphabet *** //
alphabet_info *create_alphabet(int radixsort_type, unsigned int alphabet_digit_length) {
	alphabet_info *alphabet = (alphabet_info *) malloc(sizeof(alphabet_info));

    if (alphabet == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    
    // Return the correct alphabet for a certain number of bits(length of each digit)
    // given, for either radix_sortMSD or radix_sortLSD. If an invalid radixsort_type
    // or invalid number of bits is given we terminate the programm with an error
    if (radixsort_type == 5) {
        alphabet->digit_length = alphabet_digit_length;
        alphabet->digit_num = pow(2, alphabet_digit_length);
        if (alphabet_digit_length == 1) {
            alphabet->mask = MSD_MASK1;
        }
        else if (alphabet_digit_length == 2) {
            alphabet->mask = MSD_MASK2;
        }
        else if (alphabet_digit_length == 4) {
            alphabet->mask = MSD_MASK3;
        }
        else if (alphabet_digit_length == 8) {
            alphabet->mask = MSD_MASK4;
        }
        else if (alphabet_digit_length == 16) {
            alphabet->mask = MSD_MASK5;
        }
        else {
            free(alphabet);
            printf("Invalid argument\n");
            exit(EXIT_FAILURE);
        }
    }
    else if (radixsort_type == 6) {
        alphabet->digit_length = alphabet_digit_length;
        alphabet->digit_num = pow(2, alphabet_digit_length);
        if (alphabet_digit_length == 1) {
            alphabet->mask = LSD_MASK1;
        }
        else if (alphabet_digit_length == 2) {
            alphabet->mask = LSD_MASK2;
        }
        else if (alphabet_digit_length == 4) {
            alphabet->mask = LSD_MASK3;
        }
        else if (alphabet_digit_length == 8) {
            alphabet->mask = LSD_MASK4;
        }
        else if (alphabet_digit_length == 16) {
            alphabet->mask = LSD_MASK5;
        }
        else {
            free(alphabet);
            printf("Invalid argument\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        free(alphabet);
        printf("Invalid argument\n");
        exit(EXIT_FAILURE);
    }

    return alphabet;
}