// TODO - free queue;

#include "quadtree.h"

/*	Functia creeaza un nod nod cu caracteristicile
	specificate in parametrii acesteia */
Node* initNode(unsigned char r, unsigned char g, unsigned char b, unsigned char x, uint32_t dim, unsigned int index)
{
	Node *p = malloc(sizeof(Node));
	p->r = r;
	p->g = g;
	p->b = b;
	p->x = x;
	p->area = dim*dim;
	p->index = index;
	p->contor = indiceVector++;
	p->childtl = NULL;
	p->childtr = NULL;
	p->childbl = NULL;
	p->childbr = NULL;
	p->isLeaf = 0;
	return p;
}

/*	Functia insereaza un nou nod in arborele 
	trimis ca parametru */
void insert (Node* root, Node * child)
{
	if (root->childtl == NULL)
		root->childtl = child;
	else if (root->childtr == NULL)
		root->childtr = child;
	else if (root->childbr == NULL)
		root->childbr = child;
	else if (root->childbl == NULL)
		root->childbl = child;
}

/*	Initializeaza o coada generica */
QueueNode * initQueueNode(void *node)
{
	QueueNode *p = malloc(sizeof(QueueNode));
	p->info = node;
	p->next = p;
	p->prev = p;
	return p;
}
/*	Adauga un element in coada generica */
void pushQueue(QueueNode ** head, void * node)
{
	QueueNode * p = *head;
	*head = initQueueNode(node);
	(*head)->next = p;
	(*head)->prev = p->prev;
	p->prev->next = (*head);
	p->prev = (*head);
}

/*	Functia elimina un element din coada */
void popQueue(QueueNode ** head)
{
	QueueNode *p = *head;
	if ((*head)->next == *head)
		*head = NULL;
	p->prev->prev->next = p;
	QueueNode *q = p->prev;
	p->prev = p->prev->prev;
	free(q);
}

/*	Functia intoarce ultimul element al cozii */
Node* peekQueue(QueueNode *head)
{
	return (Node *)(head->prev->info);
}