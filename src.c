#include "matrix.h"

int main(int argc, char *argv[])
{

	Pixel a, b;
	if (strcmp(argv[1], "-r") == 0)
	{
		imgToRead = fopen(argv[3], "rb");
		imgToWrite = fopen(argv[4], "wb");		
	}
	else if (strcmp(argv[1], "-b") == 0)
	{
		imgToRead = fopen(argv[8], "rb");
		imgToWrite = fopen(argv[9], "wb");	
	 	a.r = atoi(argv[2]);
	 	a.g = atoi(argv[3]);
	 	a.b = atoi(argv[4]);
	 	b.r = atoi(argv[5]);
	 	b.g = atoi(argv[6]);
	 	b.b = atoi(argv[7]);
	 	a.x = 255;
	 	b.x = 255;
	}
	else
	{
		imgToRead = fopen(argv[2], "rb");
		imgToWrite = fopen(argv[3], "wb");
	}
	if (imgToRead == NULL)
		return -1;

	/* Copiem headerul si extragem width si height */
	unsigned char *buffer = malloc(SIZE_HEADER * sizeof(unsigned char));
	fread(buffer, 1, SIZE_HEADER, imgToRead);
	fwrite(buffer, 1, SIZE_HEADER, imgToWrite);
	free(buffer);

	fseek(imgToRead, WPOSITION, SEEK_SET);
	fread(&width, sizeof(unsigned int), 1, imgToRead);
	fread(&height, sizeof(unsigned int), 1, imgToRead);
	
	fseek(imgToRead, 0, SEEK_END);
	fseek(imgToRead, SIZE_HEADER, SEEK_SET);

	if (strcmp(argv[1], "-c") == 0) 
	{
		/* Citim matricea de pixel si parsam */
		pixelArray = malloc(width*height*sizeof(Pixel));
		buffer = malloc(4*sizeof(unsigned char)); //buffer culori
		fread(pixelArray, sizeof(Pixel), width*width, imgToRead);

		createTreeFromImage(pixelArray); // creeaza arboele
		free(buffer);
		free(pixelArray);
		fseek(imgToWrite, SIZE_HEADER, SEEK_SET);

		fwrite(&nrLeafs, sizeof(uint32_t), 1, imgToWrite);
		fwrite(&indiceVector, sizeof(uint32_t), 1, imgToWrite);

		QuadtreeNode x = nodeToVectorElement(treeRoot);	
		fwrite(&x, sizeof(QuadtreeNode), 1, imgToWrite);
		makeCompressedFile(treeRoot); // se scrie arborele in fisierul binar.
	}
	else if (strcmp(argv[1], "-d") == 0)
	{
		buffer = malloc(4*width*width*sizeof(unsigned char));
		fread(&nrLeafs, sizeof(int), 1, imgToRead);
		fread(&indiceVector, sizeof(int), 1, imgToRead);
		fileToTree(); // decompress
		printBonus(treeRoot, buffer, 0, sqrt(treeRoot->area));	
		fwrite(buffer, sizeof(unsigned char), 4*width*width, imgToWrite);
		free(buffer);
	}
	else if (strcmp(argv[1], "-r") == 0)
	{
		int nr_rot = (atoi(argv[2]) % 4);
		pixelArray = malloc(width*height*sizeof(Pixel));
		fread(pixelArray, sizeof(Pixel), width*width, imgToRead);	
		createTreeFromImage(pixelArray);
		free(pixelArray);
		fseek(imgToWrite, SIZE_HEADER, SEEK_SET);
		if (nr_rot != 0)
			rotate(treeRoot, nr_rot%4);
		buffer = malloc(4*width*width*sizeof(unsigned char));
		printBonus(treeRoot, buffer, 0, sqrt(treeRoot->area));	
		fwrite(buffer, sizeof(unsigned char), 4*width*width, imgToWrite);
		free(buffer);
	}
	else if (strcmp(argv[1], "-b") == 0)
	{
		pixelArray = malloc(width*height*sizeof(Pixel));
		fread(pixelArray, sizeof(Pixel), width*width, imgToRead);	
		createTreeFromImage(pixelArray); // se creeaza arboele pe baza imaginii primite
		free(pixelArray);
		Node *stramos = findCommonParent(treeRoot, a,b); // cautam stramosul comun
		width = sqrt(stramos->area);
		fseek(imgToWrite, WPOSITION, SEEK_SET);
		fwrite(&width, sizeof(int), 1, imgToWrite);
		fwrite(&width, sizeof(int), 1, imgToWrite);
		fseek(imgToWrite, SIZE_HEADER, SEEK_SET);
		buffer = malloc(4*stramos->area*sizeof(unsigned char));
		printBonus(stramos, buffer, 0, sqrt(stramos->area));	
		fwrite(buffer, sizeof(unsigned char), 4*width*width, imgToWrite);
		free(buffer);
	}

	fclose(imgToRead);
	fclose(imgToWrite);
	return 0;
}

/*	Functia reconstruieste arborele pe baza fisierului comprimat */
void fileToTree()
{
	QuadtreeNode *vector = malloc(indiceVector * sizeof(QuadtreeNode));
	fread(vector, sizeof(QuadtreeNode), indiceVector, imgToRead);
	treeRoot = initNode(vector[0].red, vector[0].green, vector[0].blue ,vector[0].reserved, sqrt(vector[0].area), 0);
	queueRoot = initQueueNode(treeRoot);
	int *j = malloc(sizeof(int));
	*j = 0;
	QueueNode *queueIndex = initQueueNode(j);

	while(queueRoot != NULL)
	{
		Node *currentNode = peekQueue(queueRoot);
		int *indexSearch = (int *)peekQueue(queueIndex);
		if (vector[*indexSearch].top_left != -1) // are copii
		{
			j = malloc(sizeof(int));
			*j = vector[*indexSearch].top_right;
			int dimensiune = sqrt(currentNode->area)/2;
			unsigned int indexStart = width*dimensiune + currentNode->index;
			Node *toAdd = initNode(vector[*j].red, vector[*j].green, vector[*j].blue ,vector[*j].reserved, dimensiune, indexStart);
			insert(currentNode, toAdd);
			pushQueue(&queueRoot, toAdd);
			pushQueue(&queueIndex, j);

			j = malloc(sizeof(int));
			*j = vector[*indexSearch].bottom_right;
			indexStart = currentNode->index + dimensiune * (width + 1);
			toAdd = initNode(vector[*j].red, vector[*j].green, vector[*j].blue ,vector[*j].reserved, dimensiune, indexStart);
			insert(currentNode, toAdd);
			pushQueue(&queueIndex, j);
			pushQueue(&queueRoot, toAdd);

			j = malloc(sizeof(int));
			*j = vector[*indexSearch].bottom_left;
			indexStart =  currentNode->index + dimensiune;
			toAdd = initNode(vector[*j].red, vector[*j].green, vector[*j].blue ,vector[*j].reserved, dimensiune, indexStart);
			insert(currentNode, toAdd);
			pushQueue(&queueIndex, j);
			pushQueue(&queueRoot, toAdd);

			j = malloc(sizeof(int));
			*j = vector[*indexSearch].top_left;
			indexStart = currentNode->index;
			toAdd = initNode(vector[*j].red, vector[*j].green, vector[*j].blue ,vector[*j].reserved, dimensiune, indexStart);
			insert(currentNode, toAdd);
			pushQueue(&queueIndex, j);
			pushQueue(&queueRoot, toAdd);

		}
		popQueue(&queueRoot);
		popQueue(&queueIndex);
		free(indexSearch);
	}

	free(vector);
}

/*	Functia intoarce imaginea de nr_rot ori in sens trigonometric */
void rotate(Node * root, int nr_rot)
{
	if (root->childbr == NULL)
		return;
	int i = 0;
	for (i = 0; i < nr_rot; i++){
		Node *aux1 = root->childtl;
		root->childtl = root->childtr;
		root->childtr = root->childbr;
		root->childbr = root->childbl;
		root->childbl = aux1;
	}	

	rotate(root->childtl, nr_rot);
	rotate(root->childtr, nr_rot);
	rotate(root->childbr, nr_rot);
	rotate(root->childbl, nr_rot);
}

/*	Functia gaseste stramosul comun intre 2 culori
	primite ca parametru sub forma unui struct Pixel */
Node *findCommonParent(Node *root, Pixel a, Pixel b)
{
	if (root == NULL)
		return NULL;
	Pixel c;
	c.r = root->r;
	c.g = root->g;
	c.b = root->b;
	c.x = 255;
	if (comparePixels(&c, &a) || comparePixels(&c, &b))
		return root;

	Node * v[4];
	v[0] = findCommonParent(root->childtl, a, b);
	v[1] = findCommonParent(root->childtr, a, b);
	v[2] = findCommonParent(root->childbl, a, b);
	v[3] = findCommonParent(root->childbr, a, b);
	Node *n1 = NULL, *n2 = NULL;
	int i = 0;
	for (i = 0; i<4; i++)
	{
		if (v[i] != NULL && n1 == NULL)
			n1 = v[i];
		else if (v[i] != NULL && n2 == NULL)
			n2 = v[i];

	}

	if (n1 != NULL && n2 != NULL)
		return root;
	if (n1 != NULL)
		return n1;
	else if (n2 != NULL)
		return n2;
	return NULL;
}

/*	Functia reconstruieste imaginea incepand de la nodul
	primit ca si parametru */
void printBonus(Node *currentNode, unsigned char *buffer, int index, int w)
{
	unsigned int dimensiune = sqrt(currentNode->area)/2;
	if (currentNode->childbr == NULL && currentNode->childbl == NULL && currentNode->childtr == NULL && currentNode->childtl == NULL)
	{
		int i = 0, j = 0;
		int dim = sqrt(currentNode->area);
		for(i = 0; i < dim; i++)
		{	
			for (j = index + i*(w); j < index + i*w + dim; j++)
					{
						*(buffer+j*4) = currentNode->b;
						*(buffer+j*4+1) = currentNode->g;
						*(buffer+j*4+2) = currentNode->r;
						*(buffer+j*4+3) = currentNode->x;
					}
		}
		free(currentNode);
		return;
	}

	printBonus(currentNode->childbl, buffer, index, w);
	printBonus(currentNode->childbr, buffer, index+dimensiune, w);
	printBonus(currentNode->childtr, buffer, index + dimensiune * (w + 1), w);
	printBonus(currentNode->childtl, buffer, w*dimensiune + index, w);
	free(currentNode);

}

/*	Functia scrie un arbore in fisierul binar
	corespunzator imaginii compresate */
void makeCompressedFile(Node *currentNode)
{
	if (currentNode->isLeaf == 1)
		return;
	QuadtreeNode x;
	x = nodeToVectorElement(currentNode->childtl);	
	fwrite(&x, sizeof(QuadtreeNode), 1, imgToWrite);
	x = nodeToVectorElement(currentNode->childtr);	
	fwrite(&x, sizeof(QuadtreeNode), 1, imgToWrite);
	x = nodeToVectorElement(currentNode->childbr);	
	fwrite(&x, sizeof(QuadtreeNode), 1, imgToWrite);
	x = nodeToVectorElement(currentNode->childbl);	
	fwrite(&x, sizeof(QuadtreeNode), 1, imgToWrite);

	makeCompressedFile(currentNode->childtl);
	makeCompressedFile(currentNode->childtr);
	makeCompressedFile(currentNode->childbr);
	makeCompressedFile(currentNode->childbl);
}

/*	Face conversia intre struct-ul Node si
	structul QuadtreeNode */
QuadtreeNode nodeToVectorElement(Node *currentNode)
{
	QuadtreeNode x;
	x.blue = currentNode->b;
	x.green = currentNode->g;
	x.red = currentNode->r;
	x.reserved = currentNode->x;
	x.area = currentNode->area;
			
	if (currentNode->isLeaf == 1)
	{
		x.top_left = -1;
		x.top_right = -1;
		x.bottom_right = -1;
		x.bottom_left = -1;
	}
	else 
	{
		x.top_right = (int32_t)currentNode->childtl->contor;
		x.bottom_right = (int32_t)currentNode->childtr->contor;
		x.bottom_left = (int32_t)currentNode->childbr->contor;
		x.top_left = (int32_t)currentNode->childbl->contor;
	}
	return x;
}