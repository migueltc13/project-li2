#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue();
bool isEmpty(Queue* queue);
void enqueue(Queue* queue, int data);
int dequeue(Queue* queue);
void destroyQueue(Queue* queue);


#endif
