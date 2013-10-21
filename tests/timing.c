#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <bostree.h>

void test_tree_sanity(BOSTree *tree) {
	int i;
	for(i=0; i<bostree_node_count(tree); i++) {
		BOSNode *node = bostree_select(tree, i);
		if(bostree_rank(node) != i) {
			printf("Rank for node %s is %d, but should be %d\n", (char*)node->key, bostree_rank(node), i);
			exit(1);
		}
		if(bostree_lookup(tree, node->key) != node) {
			printf("Lookup for %s returned the wrong node.\n", (char*)node->key);
			exit(1);
		}
		if(bostree_next_node(node) != bostree_select(tree, i + 1)) {
			printf("Next node is %s, but select (i+1) is %s\n", (char*)bostree_next_node(node)->key, (char*)bostree_select(tree, i + 1)->key);
			exit(1);
		}
		if(bostree_previous_node(node) != bostree_select(tree, i - 1)) {
			printf("Next node is %s, but select (i+1) is %s\n", (char*)bostree_next_node(node)->key, (char*)bostree_select(tree, i + 1)->key);
			exit(1);
		}
		if(node->parent_node != NULL) {
			if(node->parent_node->left_child_node != node && node->parent_node->right_child_node != node) {
				printf("Node %s has a parent, %s, but is not one of its children.\n", (char*)node->key, (char*)node->parent_node->key);
				exit(1);
			}
		}
	}
}

int main() {
	int i;
	BOSTree *tree = bostree_new((BOSTree_cmp_function)strcmp);
	struct timeb t1, t2;
	float gestime = 0;

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
