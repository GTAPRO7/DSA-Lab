#include <stdlib.h>
#include <stdio.h>
#include "orderqueue.h"

OrderQueue* createQueue() {
    OrderQueue* q = (OrderQueue*)malloc(sizeof(OrderQueue));
    if (!q) return NULL;
    q->head = NULL;
    q->tail = NULL;
    q->current_size = 0;
    return q;
}

void enqueue(OrderQueue* q, Order order) {
    if (!q) return;

    // Allocate exactly one node. Bounded O(1) work.
    OrderNode* newNode = (OrderNode*)malloc(sizeof(OrderNode));
    if (!newNode) {
        fprintf(stderr, "Critical: Memory allocation failed on enqueue.\n");
        return; 
    }
    newNode->order = order;
    newNode->next = NULL;

    if (q->tail == NULL) {
        q->head = newNode;
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->current_size++;
}

bool dequeue(OrderQueue* q, Order* out_order) {
    if (!q || isEmpty(q)) return false;

    OrderNode* temp = q->head;
    if (out_order) *out_order = temp->order;

    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL; // Queue is now empty
    }

    // Free exactly one node. Immediately shrinks memory footprint.
    free(temp);
    q->current_size--;
    
    return true;
}

bool peek(OrderQueue* q, Order* out_order) {
    if (!q || isEmpty(q)) return false;
    
    if (out_order) *out_order = q->head->order;
    return true;
}

bool isEmpty(OrderQueue* q) {
    return (q == NULL || q->head == NULL);
}

void destroyQueue(OrderQueue* q) {
    if (!q) return;
    Order dummy;
    while (!isEmpty(q)) {
        dequeue(q, &dummy);
    }
    free(q);
}