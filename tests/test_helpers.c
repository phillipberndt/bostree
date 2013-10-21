#include <stdio.h>
#include <stdlib.h>
#include "test_helpers.h"

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

		int left_depth = node->left_child_node != NULL ? node->left_child_node->depth + 1 : 0;
		int right_depth = node->right_child_node != NULL ? node->right_child_node->depth + 1 : 0;
		if(abs(left_depth - right_depth) > 1) {
			printf("Node %s violates the balance constraint: %d left depth vs. %d right depth\n", (char *)node->key, left_depth, right_depth);
			exit(1);
		}
	}
}

