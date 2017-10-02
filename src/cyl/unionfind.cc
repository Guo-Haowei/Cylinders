#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "unionfind.h"

UnionFindNode* Find(UnionFindNode* e) {
	UnionFindNode* root=e;
	while (root != root->parent) {
		root = root->parent;
	}
	while (e != root) {
		UnionFindNode* p=e->parent;
		e->parent = root;
		e = p;
	}
	return root;
}

void Union(UnionFindNode* e1, UnionFindNode* e2) {
	UnionFindNode* root1;
	UnionFindNode* root2;
	root1 = Find(e1);
	root2 = Find(e2);
	if ( root1 != root2 ) {
		root2->parent = root1;
	}
}

void InitUnionFindNodes(UnionFindNode UL[], int n) {
	int i;
	for (i=0; i<n; i++) {
		UL[i].i = i;
		UL[i].parent = &(UL[i]);
	}
}
