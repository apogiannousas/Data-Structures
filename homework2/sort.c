#include <stdio.h>
#include "sort.h"

// *** insertion_sort *** //
void insertion_sort(dlist *list) {
	dlist_node *leftpart_node, *rightpart_node;
	int i = 1, pivot;

	// Check if list is empty
	if (list == NULL) {
		fprintf(stderr, "List is empty\n");
		return;
	}
	
	// Check if list has at least one element
	if (list->size < 1) {
		return;
	}

	// Insert each element of the unsorted part at its right position in the 
	// sorted part. Note that sorted part is on the left before the unsorted part  
	for (rightpart_node = list->head->nxt->nxt; rightpart_node != list->tail; rightpart_node = rightpart_node->nxt, i++) {
		pivot = rightpart_node->data;
		leftpart_node = rightpart_node->prev;

		while (leftpart_node != list->head && leftpart_node->data > pivot) {
			leftpart_node->nxt->data = leftpart_node->data;
			leftpart_node = leftpart_node->prev;
		}
		leftpart_node->nxt->data = pivot;
		
		// Print list in the current phase of the algorithm
		printf(" [%d]: ", i);
		print_list(*list);
	}
}

// *** selection_sort *** //
void selection_sort(dlist *list) {
	dlist_node *leftpart_node, *rightpart_node, *min;
	int i = 1, temp;

	// Check if list is empty
	if (list == NULL) {
		fprintf(stderr, "List is empty\n");
		return;
	}
	
	// Check if list has at least two elements
	if (list->size < 2) {
		return;
	}

	// Find the minimum element of the unsorted part and put it 
	// at the end of the sorted part until unsorted part is empty
	for (leftpart_node = list->head->nxt; leftpart_node != list->tail->prev; leftpart_node = leftpart_node->nxt, i++) {
		min = leftpart_node;
		for (rightpart_node = leftpart_node->nxt; rightpart_node != list->tail; rightpart_node = rightpart_node->nxt) {
			if (rightpart_node->data < min->data) {
				min = rightpart_node;
			}
		}
		temp = leftpart_node->data;
		leftpart_node->data = min->data;
		min->data = temp;

		// Print list in the current phase of the algorithm
		printf(" [%d]: ", i);
		print_list(*list);

	}
}

// *** print_end_info *** //
void print_quicksort_info(dlist_node *start, dlist_node *end, int recursion_num, char info_type) {
	dlist_node *curr;
	int i;
	
	// Print list info for this phase of quicksort recursion
	for (i = 0; i < recursion_num; i++) {
		printf("== ");
	}
	printf("[%d %c] ", recursion_num, info_type);

    for (curr = start; curr != end->nxt; curr = curr->nxt) {
        printf("%d ", curr->data);
    }
    putchar('\n');
}

// *** quick_sort *** //
void quick_sort(dlist *list, int left, int right, int recursion_num) {
	dlist_node *left_node, *right_node, *pivot;
	dlist_node *left_temp_node, *right_temp_node;
	int i = left - 1, j = right, temp;
	
	if (left >= right) {
		return;
	}

	left_node  = find_node_inplace(*list, left + 1);
	right_node = find_node_inplace(*list, right + 1);
	print_quicksort_info(left_node, right_node, recursion_num, '>');

	// Put all elements smaller than pivot
	// on its left and the bigger on its right
	pivot = right_node;
	left_temp_node  = left_node->prev;
	right_temp_node = right_node;
	while (1) {
		do {
			left_temp_node = left_temp_node->nxt;
			i++;
		} while (left_temp_node->data < pivot->data);

		do {
			if (j != left) {
				right_temp_node = right_temp_node->prev;
				j--;
			}
			else {
				break;
			}
		} while (right_temp_node->data > pivot->data);

		if (i >= j) {
			break;
		}

		temp = left_temp_node->data;
		left_temp_node->data = right_temp_node->data;
		right_temp_node->data = temp;

	}
	temp = left_temp_node->data;
	left_temp_node->data = pivot->data;
	pivot->data = temp;

	// Put pivot in its right place and then
	// sort the left and right subarray which
	// are separated from it in the same way
	if ((right - left + 1) > 2) {
		print_quicksort_info(left_node, right_node, recursion_num, '-');
	}
	quick_sort(list, left, i - 1, recursion_num + 1);
	quick_sort(list, i + 1, right, recursion_num + 1);
	print_quicksort_info(left_node, right_node, recursion_num, '<');
}

// *** print_mergesort_info *** //
void print_mergesort_info(dlist list, int recursion_num, char info_type) {
	int i;
	
	// Print list info at the end of this recursion
	for (i = 0; i < recursion_num; i++) {
		printf("== ");
	}
	printf("[%d %c] ", recursion_num, info_type);
	print_list(list);
}

// *** split_list *** //
dlist *split_list(dlist *list1) {
	int middle = list1->size / 2;
	dlist *list2 = create_list();
	dlist_node *right_node = list1->tail->prev;
	dlist_node *mid_node = find_node_inplace(*list1, middle);

	// Check if middle node was found;
	if (mid_node == NULL) {
		fprintf(stderr, "Error in split_list: failed to find mid_node\n");
		return NULL;
	}

	// Make the node after the node in the middle the new starting node of list2
	mid_node->nxt->prev = list2->head;
	list2->head->nxt = mid_node->nxt;

	// Make the rightmost node the new ending node of list2
	right_node->nxt = list2->tail;
	list2->tail->prev = right_node;

	// Make the node in the middle the new ending node of list1
	mid_node->nxt = list1->tail;
	list1->tail->prev = mid_node;

	// Give both lists their actual sizes
	list2->size = list1->size - middle;
	list1->size = middle;

	return list2;
}

// *** merge_lists *** //
void merge_lists(dlist *list1, dlist *list2) {
	dlist_node *curr1 = list1->head->nxt;
	dlist_node *curr2 = list2->head->nxt;

	// Attach nodes of list2 to the right place at list1
	// until either list2 is empty or the biggest element of
	// list1 is smaller than the smallest element of list2
	while (curr2 != list2->tail && curr1 != list1->tail) {
		if (curr2->data < curr1->data) {
			// Extract curr2 from list2
			list2->head->nxt = curr2->nxt;
			list2->head->nxt->prev = list2->head;

			// Attach curr2 to list1 before curr1
			curr2->prev = curr1->prev;
			curr2->prev->nxt = curr2;
			curr2->nxt	= curr1;
			curr1->prev	= curr2;
			
			// Go to the next node of list2
			curr2 = list2->head->nxt;
		}
		else {
			// Go to the next node of list1
			curr1 = curr1->nxt;
		}
	}

	// If the biggest element of list1 is smaller than the smallest element
	// of list2 simply merge those lists together, otherwise they are already merged
	if (curr1 == list1->tail) {
		// Extract the remaining nodes of list2 and attach them to list1
		curr1 = curr1->prev;
		curr1->nxt = list2->head->nxt;
		curr1->nxt->prev = curr1;
		list1->tail->prev = list2->tail->prev;
		list1->tail->prev->nxt = list1->tail;
	}

	// Delete list2
	list2->head->nxt  = list2->tail;
    list2->tail->prev = list2->head;
	delete_list(list2);
}

// *** merge_sort *** //
void merge_sort(dlist *list1, int recursion_num) {
	dlist *list2 = NULL;
	int temp;

	print_mergesort_info(*list1, recursion_num, '>');

	// If a list has two elements ensure it is sorted
	if (list1->size == 2) {
		// Swap the elements if the list is unsorted
		if (list1->head->nxt->data > list1->tail->prev->data) {
			temp = list1->head->nxt->data;
			list1->head->nxt->data = list1->tail->prev->data;
			list1->tail->prev->data = temp;
		}
		print_mergesort_info(*list1, recursion_num, '<');
		return;
	}
	
	// A list with one element is already sorted
	if (list1->size == 1) {
		print_mergesort_info(*list1, recursion_num, '<');
		return;
	}

	// Split the list into two lists, repeat the algorithm and
	// then merge the final sorted lists into one final sorted list
	list2 = split_list(list1);
	merge_sort(list1, recursion_num + 1);
	merge_sort(list2, recursion_num + 1);
	merge_lists(list1, list2);
	print_mergesort_info(*list1, recursion_num, '<');
}
