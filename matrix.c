#include "matrix.h"

/*	Functia compara 2 pixeli si intoarce 1
	daca sunt egali, sau 0 in caz contrat */

int comparePixels(void * a, void * b)
{
	Pixel *_a = a;
	Pixel *_b = b;
	if ((_a->b != _b->b) || (_a->g != _b->g) || (_a->r != _b->r) || (_a->x != _b->x))
		return 0;
	return 1;
}

/*	Verifica daca o bucata din matricea
	de pixeli contine o singura culoare. 
	Daca da, intoarce 1. In caz contrar va returna 0 */

int isMatrixUniform(Pixel array[], unsigned int dimensiune, unsigned int index_start)
{
	unsigned int i,j;
	Pixel first = array[index_start];
	for(i = 0; i < dimensiune; i++)
	{	
		for (j = index_start + i*(width); j < index_start + i*width + dimensiune; j++)
			if (comparePixels(&first, (array+j)) == 0)
				return 0;
	}
	return 1;
}

/*	Functia creeaza copiii pentru un nod primit
	ca parametru in functie */
void print(Pixel array[],unsigned int dimensiune, unsigned int index_start, Node * root)
{
	if (dimensiune == 0)
		return;
		Node * currentNode = initNode(array[index_start].r, array[index_start].g, 
									  array[index_start].b, array[index_start].x, 
									  (uint32_t)dimensiune, index_start);
		insert(root, currentNode);

	if (isMatrixUniform(array, dimensiune, index_start) == 1)

	{	currentNode->isLeaf = 1;
		nrLeafs++;
		return;
	}
}

/*	Functia primeste ca parametru un vector de pixeli
	si creeaza arborele corespunzator */

void createTreeFromImage(Pixel array[])
{
	if (isMatrixUniform(array, width, 0) == 1)
	{
		treeRoot = initNode(array[0].r, array[0].g, array[0].b, array[0].x, (uint32_t)(width), 0);
		treeRoot->isLeaf = 1;
		nrLeafs++;
		return;
	}
	
	treeRoot = initNode(array[0].r, array[0].g, array[0].b, array[0].x, (uint32_t)(width),0);
	queueRoot = initQueueNode(treeRoot);
	createTree(treeRoot);

}

/*	Functia ce ajuta construirea arborelui */
void createTree(Node *currentNode)
{
	if (currentNode == NULL || currentNode->isLeaf == 1)
		return;
	unsigned int dimensiune = sqrt(currentNode->area)/2;
	print(pixelArray, dimensiune, width*dimensiune + currentNode->index, currentNode);
	print(pixelArray, dimensiune, currentNode->index + dimensiune * (width + 1), currentNode);
	print(pixelArray, dimensiune, currentNode->index + dimensiune, currentNode);
	print(pixelArray, dimensiune, 0 + currentNode->index, currentNode);

	createTree(currentNode->childtl);
	createTree(currentNode->childtr);
	createTree(currentNode->childbr);
	createTree(currentNode->childbl);

}