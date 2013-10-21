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
	BOSTree *tree = bostree_new((BOSTree_cmp_function)strcmp);

	char iter;
	printf("Insert test:\n");
	for(iter='A'; iter<='Z'; iter++) {
		char *data = malloc(2);
		*data = iter;
		data[1] = 0;
		printf("Insert %s, ", data);
		bostree_insert(tree, data, "Value");

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
		if(node != NULL) {
			free(node->key);
		}
		else {
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
