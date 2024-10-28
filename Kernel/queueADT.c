#include "./include/queueADT.h"
#include "./include/memoryManager.h"

typedef struct Node
{
    QueueType data;
    struct Node *next;
} Node;

typedef struct QueueCDT
{
    Node *first;
    Node *last;
} QueueCDT;

QueueADT newQueue(void)
{
    QueueADT queue = (QueueADT)mallocMM(sizeof(QueueCDT));
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

void queue(QueueADT queue, QueueType elem)
{
    Node *newNode = (Node *)mallocMM(sizeof(Node));
    newNode->data = elem;
    newNode->next = NULL;
    if (queue->first == NULL)
    {
        queue->first = newNode;
        queue->last = newNode;
    }
    else
    {
        queue->last->next = newNode;
        queue->last = newNode;
    }
}

QueueType dequeue(QueueADT queue)
{
    if (queue->first == NULL)
    {
        return -1;
    }
    Node *firstNode = queue->first;
    QueueType data = firstNode->data;
    queue->first = firstNode->next;
    freeMM(firstNode);
    return data;
}

int isEmpty(QueueADT queue)
{
    return queue->first == NULL;
}

void freeQueue(QueueADT queue)
{
    while (!isEmpty(queue))
    {
        dequeue(queue);
    }
    freeMM(queue);
}