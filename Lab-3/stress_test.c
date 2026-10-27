#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "orderqueue.h"

// Helper to get time in nanoseconds
long long get_nanos() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000000000LL + ts.tv_nsec;
}

int main() {
    srand(time(NULL));
    OrderQueue* queue = createQueue();
    
    int total_operations = 1000000; // 1 million simulated operations
    long long max_enqueue_time = 0;
    long long max_dequeue_time = 0;
    long long total_enqueue_time = 0;
    long long total_dequeue_time = 0;
    int enqueue_count = 0;
    int dequeue_count = 0;

    printf("Starting stress test with %d operations...\n", total_operations);

    for (int i = 0; i < total_operations; i++) {
        // 60% chance to enqueue, 40% chance to dequeue to ensure the queue grows and fluctuates
        int r = rand() % 100;
        
        if (r < 60) {
            Order o = { .order_id = i };
            long long start = get_nanos();
            enqueue(queue, o);
            long long end = get_nanos();
            
            long long duration = end - start;
            if (duration > max_enqueue_time) max_enqueue_time = duration;
            total_enqueue_time += duration;
            enqueue_count++;
        } else {
            if (!isEmpty(queue)) {
                Order o;
                long long start = get_nanos();
                dequeue(queue, &o);
                long long end = get_nanos();
                
                long long duration = end - start;
                if (duration > max_dequeue_time) max_dequeue_time = duration;
                total_dequeue_time += duration;
                dequeue_count++;
            }
        }

        // Print a snapshot occasionally to show fluctuation
        if (i > 0 && i % 200000 == 0) {
            printf("Checkpoint: %d ops completed. Current queue size: %ld\n", i, queue->current_size);
        }
    }

    printf("\n--- LATENCY RESULTS ---\n");
    printf("Total Enqueues: %d\n", enqueue_count);
    printf("Average Enqueue: %lld ns\n", total_enqueue_time / enqueue_count);
    printf("Max Enqueue Spike: %lld ns\n", max_enqueue_time);
    
    printf("\nTotal Dequeues: %d\n", dequeue_count);
    printf("Average Dequeue: %lld ns\n", total_dequeue_time / dequeue_count);
    printf("Max Dequeue Spike: %lld ns\n", max_dequeue_time);
    printf("Final Queue Size: %ld\n", queue->current_size);

    destroyQueue(queue);
    return 0;
}