#include <stdio.h>
#include <stdlib.h>
#include "dlist.h"

//-----------Structs-----------//
typedef struct sequence_t {
    dlist_node *start_node;
    dlist_node *end_node;
    int start_index;
    int end_index;
    int size;
} sequence_t;

//-----Function Prototypes-----//
sequence_t *find_sequence(dlist list);
void print_sequence(sequence_t sequence);

//------------Main-------------//
int main(int argc, char *argv[]) {
    dlist *list = create_list();
    sequence_t *sequence = NULL;
    int data = 0;

    scanf("%d",&data);
    while (data != 0) {
        insert_node(list, data);
        scanf("%d", &data);
    }
    
    sequence = find_sequence(*list);
    if (sequence == NULL) {
        printf("There isn't a sequence whose sum equals zero\n");
    }
    else {
        print_sequence(*sequence);
    }

    delete_list(list);
    free(sequence);

    return 0;
}

//---Funtion Implementations---//

// *** find_sequence *** //
// if successful it returns the largest sequence  //
// with a sum equal to zero, else it returns NULL //
sequence_t *find_sequence(dlist list) {
    sequence_t *sequence = NULL;
    dlist_node *start, *end, *prev_start, *prev_end;
    int i, initial_size, curr_size, sum = 1;

    // Initialise start, end and size of the sequence
    start = list.head->nxt;
    end = list.tail->prev;
    prev_start = start;
    prev_end = end;
    initial_size = list.size;
    curr_size = list.size;

    while (curr_size != 0) {
        for (i = 0; i <= initial_size - curr_size; i++) {
            sum = list_sum(start, end);
            
            // Check if sequence's sum is zero and if so return it
            if (sum == 0) {
                sequence = (sequence_t *) malloc(sizeof(sequence_t));
                // Store nodes at start and end
                sequence->start_node = start;
                sequence->end_node = end;
                // Store their indicies
                sequence->start_index = i;
                sequence->end_index = i + curr_size - 1;
                // Store size
                sequence->size = curr_size;

                return sequence;
            }

            // Check the next sequence of the same size moving from left to the right
            start = start->nxt;
            end = end->nxt;
        }

        // Start checking sequences that are smaller by one in size
        prev_end = prev_end->prev;
        start = prev_start;
        end = prev_end;
        curr_size--;
    }

    return sequence;
}

// *** print_sequence *** //
void print_sequence(sequence_t sequence) {
    dlist_node *curr = sequence.start_node;

    printf("start: %d, stop: %d, size: %d\n", sequence.start_index, sequence.end_index, sequence.size);
    
    for (int i = 0; i < sequence.size; i++) {
        printf("%d ", curr->data);
        curr = curr->nxt;
    }

    putchar('\n');
}