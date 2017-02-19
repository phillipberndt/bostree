#define _GNU_SOURCE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>
#include <time.h>
#include "test_helpers.h"

int main(int argc, char *argv[]) {
	int i;
	BOSTree *tree = bostree_new((BOSTree_cmp_function)strcmp, NULL);
	struct timeb t1, t2;
	float gestime = 0;

	if(argc > 1) {
		time_t seed = atol(argv[1]);
		printf("Using seed %lu from command line\n", seed);
		srand(seed);
	}
	else {
		time_t seed = time(NULL);
		printf("Using seed %lu\n", seed);
		srand(seed);
	}

	for(i=0; i<10000000; i++) {
		char *name = malloc(32);
		int j;
		do {
			for(j=0; j<32; j++) {
				name[j] = 'A' + (unsigned long)rand() * (unsigned long)('Z' - 'A') / RAND_MAX;
			}
			name[32] = 0;
		} while(bostree_lookup(tree, name) != NULL);

		ftime(&t1);
		bostree_insert(tree, name, 0);
		ftime(&t2);

		gestime += (t2.time - t1.time ) * 1000 + t2.millitm - t1.millitm;

		if(i % 1000000 == 0) {
			printf("%07d elements, inserting a value takes %1.5e s/op\n", bostree_node_count(tree), gestime / 1e9);
			test_tree_sanity(tree);
			printf(" sanity check passed\n");
			gestime = 0;
		}
	}

	float selecttime = 0;
	float deletetime = 0;

	for(; i>0; i--) {
		int w = rand() * i / RAND_MAX;
		ftime(&t1);
		BOSNode *node = bostree_select(tree, w);
		ftime(&t2);
		selecttime += (t2.time - t1.time ) * 1000 + t2.millitm - t1.millitm;

		if(!node) {
			printf("Node missing!\n");
			test_tree_sanity(tree);
			exit(1);
		}

		ftime(&t1);
		bostree_remove(tree, node);
		ftime(&t2);
		deletetime += (t2.time - t1.time ) * 1000 + t2.millitm - t1.millitm;

		if(i % 1000000 == 0) {
			printf("%07d elements, selecting a value takes %1.5e s/op, removing a value takes %1.5e s/op\n", bostree_node_count(tree), selecttime / 1e9, deletetime / 1e9);
			test_tree_sanity(tree);
			printf(" sanity check passed\n");
			selecttime = 0;
			deletetime = 0;
		}
	}

	return 0;
}
