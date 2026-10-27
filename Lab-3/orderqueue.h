#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include <stdbool.h>

// Define what an order looks like
typedef struct {
    long order_id;
    // Other order details (items, customer_id, etc.) could go here
} Order;

// Linked list node for the queue
typedef struct OrderNode {
    Order order;
    struct OrderNode* next;
} OrderNode;

// Queue structure tracking head, tail, and current size
typedef struct {
    OrderNode* head;
    OrderNode* tail;
    long current_size;
} OrderQueue;

// Public API
OrderQueue* createQueue();
void enqueue(OrderQueue* q, Order order);
bool dequeue(OrderQueue* q, Order* out_order);
bool peek(OrderQueue* q, Order* out_order);
bool isEmpty(OrderQueue* q);
void destroyQueue(OrderQueue* q);

#endif