typedef struct unionfindnode {
	struct unionfindnode* parent;
	int i;
	int color;
} UnionFindNode;

UnionFindNode* Find(UnionFindNode* e);
void Union(UnionFindNode* e1, UnionFindNode* e2);
void InitUnionFindNodes(UnionFindNode UL[], int n);
void S3dColor(int i);

