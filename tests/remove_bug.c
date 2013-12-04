#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>
#include "test_helpers.h"

int main() {
	BOSTree *t = bostree_new((BOSTree_cmp_function)strcmp);
	bostree_insert(t, "AA", NULL);
	bostree_insert(t, "C", NULL);
	bostree_insert(t, "E", NULL);
	bostree_insert(t, "G", NULL);
	bostree_insert(t, "I", NULL);
	bostree_insert(t, "K", NULL);
	bostree_insert(t, "M", NULL);
	bostree_insert(t, "O", NULL);
	bostree_insert(t, "Q", NULL);
	bostree_insert(t, "D", NULL);
	bostree_insert(t, "F", NULL);
	bostree_insert(t, "A", NULL);

	bostree_remove(t, bostree_lookup(t, "C"));
	test_tree_sanity(t);

	return 0;
}
