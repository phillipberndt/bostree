#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include "test_helpers.h"

static unsigned int _actual_depth(BOSNode *node) {
	int left_depth = node->left_child_node ? _actual_depth(node->left_child_node) + 1 : 0;
	int right_depth = node->right_child_node ? _actual_depth(node->right_child_node) + 1 : 0;

	return left_depth > right_depth ? left_depth : right_depth;
}

static unsigned int _actual_count(BOSNode *node) {
	int left_count = node->left_child_node ? _actual_count(node->left_child_node) : 0;
	int right_count = node->right_child_node ? _actual_count(node->right_child_node) : 0;

	return left_count + right_count + 1;
}

void test_tree_sanity(BOSTree *tree) {
	unsigned int i;
	unsigned int count = 0;
	int error = 0;
	BOSNode *node;
	for(node = bostree_select(tree, 0); node; node = bostree_next_node(node)) {
		count++;
	}
	if(count != bostree_node_count(tree)) {
		printf("Manual node count yielded %d, order statistics reported %d\n", count, bostree_node_count(tree));
		exit(1);
	}
	for(i=0; i<bostree_node_count(tree); i++) {
		node = bostree_select(tree, i);
		if(node == NULL) {
			printf("Rank %d node not found, though count is %d\n", i, bostree_node_count(tree));
			error = 1;
		}
		if(bostree_rank(node) != i) {
			printf("Rank for node %s is %d, but should be %d\n", (char*)node->key, bostree_rank(node), i);
			error = 1;
		}
		if(bostree_lookup(tree, node->key) != node) {
			printf("Lookup for %s returned the wrong node.\n", (char*)node->key);
			error = 1;
		}
		if(bostree_next_node(node) != bostree_select(tree, i + 1)) {
			printf("Next node is %s, but select (i+1) is %s\n", (char*)bostree_next_node(node)->key, (char*)bostree_select(tree, i + 1)->key);
			error = 1;
		}
		if(bostree_previous_node(node) != bostree_select(tree, i - 1)) {
			printf("Next node is %s, but select (i+1) is %s\n", (char*)bostree_next_node(node)->key, (char*)bostree_select(tree, i + 1)->key);
			error = 1;
		}
		if(node->parent_node != NULL) {
			if(node->parent_node->left_child_node != node && node->parent_node->right_child_node != node) {
				printf("Node %s has a parent, %s, but is not one of its children.\n", (char*)node->key, (char*)node->parent_node->key);
				error = 1;
			}
		}
		unsigned int depth = _actual_depth(node);
		if(depth != node->depth) {
			printf("Node %s reports depth %d when the actual depth is %d\n", (char *)node->key, node->depth, depth);
			error = 1;
		}
		if(node->left_child_node) {
			unsigned left_count = _actual_count(node->left_child_node);
			if(left_count != node->left_child_count) {
				printf("Node %s reports %d left children when there are %d\n", (char *)node->key, node->left_child_count, left_count);
				error = 1;
			}
		}
		if(node->right_child_node) {
			unsigned right_count = _actual_count(node->right_child_node);
			if(right_count != node->right_child_count) {
				printf("Node %s reports %d right children when there are %d\n", (char *)node->key, node->right_child_count, right_count);
				error = 1;
			}
		}
		unsigned left_depth = node->left_child_node != NULL ? node->left_child_node->depth + 1 : 0;
		unsigned right_depth = node->right_child_node != NULL ? node->right_child_node->depth + 1 : 0;
		if(abs((int)left_depth - right_depth) > 1) {
			printf("Node %s violates the balance constraint: %d left depth vs. %d right depth\n", (char *)node->key, left_depth, right_depth);
			error = 1;
		}
	}
	if(error > 0) {
		fprintf(stderr, "Exiting due to errors.\n");
		abort();
	}
}

