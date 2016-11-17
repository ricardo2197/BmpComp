#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct QuadtreeNode
{
    unsigned char blue, green, red, reserved;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed));


struct node
{
	unsigned char r,g,b,x;
    uint32_t area;
    struct node *childtl;
    struct node *childtr;
    struct node *childbl;
    struct node *childbr;
    unsigned int index;
    unsigned int contor;
    unsigned int isLeaf;
}  __attribute__((packed));

struct q
{
    void *info;
    struct q *next;
    struct q *prev;
}  __attribute__((packed));

typedef struct node Node;
typedef struct q QueueNode;
typedef struct QuadtreeNode QuadtreeNode;
uint32_t indiceVector;
uint32_t nrLeafs;

Node* initNode(unsigned char r, unsigned char g, unsigned char b, unsigned char x, uint32_t dim, unsigned int index);
void insert (Node* root, Node * child);
QueueNode * initQueueNode(void *node);
void pushQueue(QueueNode ** head, void *node);
void popQueue(QueueNode ** head);
void unitTesting();
Node* peekQueue(QueueNode *head);

#endif