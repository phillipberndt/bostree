/*

	This test was created to monitor a bug where upon deletion of a node the
	replacer's left child tree vanished:

	For example, consider

	                 H
	           D       -something-
	      B      F
	    A   C   E  G
	       B'

	with the bug, if D was removed, C would be the replacer element, because it
	is the largest one in the left sub tree. C would take D's place and B would
	be attached as C's left child. The bug is that this way, B' vanishes.

	The fix is to attach B' as the right child of B, i.e. replace C with it's left
	child. C can not have a right child, because then it would not be the largest
	left child of D.

	This test creates a big tree and then tries to remove every single node
	from the full tree, each time checking the result for validity.

*/
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>
#include "test_helpers.h"

BOSTree *test_tree() {
	BOSTree *t = bostree_new((BOSTree_cmp_function)strcmp, NULL);
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
