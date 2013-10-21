/*
	Self-Balancing order statistic tree
	Test application

	Copyright (c) 2013, Phillip Berndt

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "bostree.h"

extern int _bostree_rotate_left(BOSTree *tree, BOSNode *node);

int main() {
	/* General consistency checks */
	BOSTree *tree = bostree_new((BOSTree_cmp_function)strcmp);

	char iter;
	for(iter='A'; iter<='Z'; iter++) {
		char *data = malloc(2);
		*data = iter;
		data[1] = 0;
		bostree_insert(tree, data, "Value");
	}

#ifndef NDEBUG
	bostree_print(tree);
#endif
	printf("\nIteration test:\n");

	int i;
	for(i=0; i<bostree_node_count(tree); i++) {
		BOSNode *node = bostree_select(tree, i);
		printf("%s, rank should be %d, is %d, lookup %d", (char*)node->key, i, bostree_rank(node), bostree_lookup(tree, node->key) == node);
		if(bostree_next_node(node) != NULL) {
			printf(", next: %s", (char*)bostree_next_node(node)->key);
		}
		if(bostree_previous_node(node) != NULL) {
			printf(", previous: %s", (char*)bostree_previous_node(node)->key);
		}
		if(node->parent_node != NULL) {
			printf(", parent: %s", (char*)node->parent_node->key);
		}
		printf("\n");
	}

	printf("\nRemove test:\n");
	for(iter='A'; iter<='Z'; iter++) {
		char data[2];
		*data = iter;
		data[1] = 0;
		printf("Remove %s, ", data);
		bostree_remove(tree, bostree_lookup(tree, data));
		printf("%d left. ", bostree_node_count(tree));
	}
	printf("\n");

	bostree_destroy(tree);

	while(fgetc(stdin) != '\n');

	/* Speed tests */

	struct timeb t1, t2;
	float gestime = 0;
	tree = bostree_new((BOSTree_cmp_function)strcmp);

	for(i=0; i<10000000; i++) {
		char *name = malloc(10);
		int j;
		for(j=0; j<10; j++) {
			name[j] = 'A' + rand() * ('Z' - 'A') / RAND_MAX;
		}
		name[10] = 0;

		ftime(&t1);
		bostree_insert(tree, name, 0);
		ftime(&t2);

		gestime += (t2.time - t1.time ) * 1000 + t2.millitm - t1.millitm;

		if(i % 1000000 == 0) {
			printf("%07d elements, inserting a value takes %1.5e s/op\n", bostree_node_count(tree), gestime / 1e9);
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
			selecttime = 0;
			deletetime = 0;
		}
	}

	return 0;
}
