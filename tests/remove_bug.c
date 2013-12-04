#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>
#include "test_helpers.h"

BOSTree *test_tree() {
	BOSTree *t = bostree_new((BOSTree_cmp_function)strcmp);
	char i;
	for(i='A'; i<'Z'; i++) {
		char *key = malloc(2);
		key[0] = i; key[1] = 0;
		bostree_insert(t, key, NULL);
	}

	return t;
}

int main() {
	char remove[2];
	remove[1] = 0;
	for(*remove='A'; *remove<'Z'; (*remove)++) {
		BOSTree *t = test_tree();
		bostree_remove(t, bostree_lookup(t, remove));
		test_tree_sanity(t);
		if(bostree_node_count(t) != ('Z' - 'A' - 1)) {
			printf("Removed one node from a tree, but the node count did not decrease properly.\n");
			exit(1);
		}
		bostree_destroy(t);
	}

	return 0;
}
