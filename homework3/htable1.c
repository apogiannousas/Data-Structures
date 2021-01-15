#include <stdio.h>
#include <stdlib.h>
#include "htable.h"

//-----Function Prototypes-----//
void init_input_str(char *input_str);

//------------Main-------------//
int main(int argc, char *argv[]) {
	enum command {insert, delete, find};
	enum command command;
    htable *table = create_htable();
	char input_str[11] = {'\0'};
    int key, data, result, i = 0;

	result = scanf("%s", input_str);
	while (result == 1) {
		if (input_str[0] == '-') {
			switch (input_str[1]) {
				case 'i': {
					command = insert;
					break;
				}
				case 'd': {
					command = delete;
					break;
				}
				case 'f': {
					command = find;
					break;
				}
				case 'p': {
					print_htable(table);
					break;
				}
				case 'q': {
					delete_htable(table);
					return 0;
				}
				default: {
					fprintf(stderr, "Error: Wrong flag -%c was given", input_str[1]);
					exit(EXIT_FAILURE);
				}
			}

			init_input_str(input_str);
			result = scanf("%s", input_str);
			continue;
		}

		data = atoi(input_str);
		switch (command) {
			case insert: {
				result = insert_entry(table, data, true);
				if (result == 1) {
					printf("\nINSERTED %d\n", data);
					i++;
				}
				else {
					printf("\nNOT INSERTED %d\n", data);
				}
				break;
			}
			case delete: {
				result = remove_entry(table, data, true);
				if (result == 1) {
					printf("\nDELETED %d\n", data);
					i++;
				}
				else {
					printf("\nNOT DELETED %d\n", data);
				}
				break;
			}
			case find: {
				result = find_entry(table, data, &key);
				if (result == 1) {
					printf("\nFOUND %d\n", data);
				}
				else {
					printf("\nNOT FOUND %d\n", data);
				}
				break;
			}
			default: {
				break;
			}
		}

		init_input_str(input_str);
		result = scanf("%s", input_str);
	}
	delete_htable(table);

    return 1;
}

//---Funtion Implementations---//

// *** init_input_str *** //
void init_input_str(char *input_str) {
	int i;

	for (i = 0; i < 11; i++) {
		input_str[i] = '\0';
	}
}