#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int main(int argc, char *argv[]) {
    dlist *list = create_list();
    int data;

    scanf("%d",&data);
    while (data != -1) {
        insert_node(list, data);
        scanf("%d", &data);
    }
    print_list(*list);
    putchar('\n');

    if (argc != 2) {
        printf("Invalid argument\n");
        return 1;
    }

    if (atoi(argv[1]) == 1) {
        insertion_sort(list);
    }
    else if (atoi(argv[1]) == 2) {
        selection_sort(list);
    }
    else if (atoi(argv[1]) == 3) {
        quick_sort(list, 0, list->size - 1, 0);
    }
    else if (atoi(argv[1]) == 4) {
        merge_sort(list, 0);
    }

    putchar('\n');
    print_list(*list);
    delete_list(list);

    return 0;
}