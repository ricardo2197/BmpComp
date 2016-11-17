#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "quadtree.h"
#define SIZE_HEADER 54
#define WPOSITION 18

FILE *imgToRead;
FILE *imgToWrite;
FILE *out;
unsigned int width, height;


typedef struct p
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char x;
}Pixel;

Pixel *pixelArray;
Node *treeRoot;
QueueNode *queueRoot;

void print(Pixel array[], unsigned int dimensiune, unsigned int index_start, Node * root);
void createTreeFromImage(Pixel array[]);
void createTree(Node *currentNode);
void fileToTree();
void rotate(Node * root, int nr_rot);
int comparePixels(void * a, void * b);
Node *findCommonParent(Node *root, Pixel a, Pixel b);
void printBonus(Node *root, unsigned char *buffer, int index, int w);
void makeCompressedFile(Node *currentNode);
QuadtreeNode nodeToVectorElement(Node *currentNode);

#endif