/*

	This test is related to remove_bug.c. If H is removed from

	                 H
	           D       -something-
	      B      F
	    A   C   E


	E was not correctly reparented.

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
	// Prepare the situation from above
	BOSTree *t = test_tree();
	bostree_remove(t, bostree_lookup(t, strdup("G")));
	bostree_remove(t, bostree_lookup(t, strdup("H")));

	test_tree_sanity(t);
	if(bostree_lookup(t, strdup("E")) == NULL) {
		printf("Nodes missing after removing another one\n");
		exit(1);
	}

	return 0;
}
