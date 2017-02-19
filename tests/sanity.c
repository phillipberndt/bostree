#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>
#include "test_helpers.h"

void free_function(BOSNode *node) {
	free(node->key);
	free(node->data);
}

int main() {
	BOSTree *tree = bostree_new((BOSTree_cmp_function)strcmp, free_function);

	char iter;
	printf("Insert test:\n");
	for(iter='A'; iter<='Z'; iter++) {
		char *data = malloc(2);
		*data = iter;
		data[1] = 0;
		printf("Insert %s, ", data);
		bostree_insert(tree, data, strdup("Value"));

		test_tree_sanity(tree);
	}

#ifndef NDEBUG
	bostree_print(tree);
#endif


	printf("\nRemove test:\n");
	for(iter='A'; iter<='Z'; iter++) {
		char data[2];
		*data = iter;
		data[1] = 0;
		printf("Remove %s, ", data);
		BOSNode *node = bostree_lookup(tree, data);
		if(node == NULL) {
			printf("Lookup for %s showed it was already deleted\n", data);
			exit(1);
		}
		bostree_remove(tree, node);

		test_tree_sanity(tree);
	}
	printf("\n");

	bostree_destroy(tree);
	exit(0);
}
