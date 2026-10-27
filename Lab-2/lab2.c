/*
 * TIME COMPLEXITY:
 * Hash Lookup: Average O(1)
 * SEARCH/RANK: O(log k)
 * INSERT: O(k) worst-case due to array shifting. Amortized O(1) for rehashing.
 * DELETE: O(k) worst-case due to array shifting.
 * 
 * SPACE COMPLEXITY: O(N + M)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DeptGroup {
    char dept[6];
    int* rolls;
    int capacity;
    int size;
    struct DeptGroup* next;
} DeptGroup;

int hashDept(const char* dept, int M) {
    unsigned int h = 0;
    for (int i = 0; dept[i] != '\0'; i++) {
        h = (h * 31 + dept[i]) % M;
    }
    return h;
}

int findPos(int* arr, int size, int val) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == val) return mid;
        else if (arr[mid] < val) low = mid + 1;
        else high = mid - 1;
    }
    return low; 
}

// Helper functions for Rehashing
int isPrime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    return 1;
}

int getNextPrime(int n) {
    int prime = n;
    while (1) {
        prime++;
        if (isPrime(prime)) return prime;
    }
}

int main() {
    int M, Q;
    printf("Enter number of buckets (M) and number of queries (Q): ");
    if (scanf("%d %d", &M, &Q) != 2) return 1;

    DeptGroup** table = (DeptGroup**)calloc(M, sizeof(DeptGroup*));
    int total_records = 0;

    char op[10], dept[6];
    int rollNo;

    for (int i = 0; i < Q; i++) {
        printf("\nEnter operation (INSERT/DELETE/SEARCH/RANK), department, and roll number: ");
        scanf("%s %s %d", op, dept, &rollNo);

        int h = hashDept(dept, M);
        DeptGroup* curr = table[h];
        DeptGroup* group = NULL;
        DeptGroup* prev = NULL;
        DeptGroup* group_prev = NULL;

        // Traverse chain tracking the 'prev' pointer for possible deletion
        while (curr != NULL) {
            if (strcmp(curr->dept, dept) == 0) {
                group = curr;
                group_prev = prev;
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if (strcmp(op, "INSERT") == 0) {
            if (group == NULL) {
                group = (DeptGroup*)malloc(sizeof(DeptGroup));
                strcpy(group->dept, dept);
                group->capacity = 4;
                group->size = 0;
                group->rolls = (int*)malloc(group->capacity * sizeof(int));
                group->next = table[h];
                table[h] = group;
            }

            int idx = findPos(group->rolls, group->size, rollNo);
            if (idx >= group->size || group->rolls[idx] != rollNo) {
                if (group->size == group->capacity) {
                    group->capacity *= 2;
                    group->rolls = (int*)realloc(group->rolls, group->capacity * sizeof(int));
                }
                for (int j = group->size; j > idx; j--) {
                    group->rolls[j] = group->rolls[j - 1];
                }
                group->rolls[idx] = rollNo;
                group->size++;
                total_records++;

                // LOAD-FACTOR TRIGGERED REHASHING (Threshold > 4.0)
                if ((float)total_records / M > 4.0) {
                    int new_M = getNextPrime(M * 2);
                    DeptGroup** new_table = (DeptGroup**)calloc(new_M, sizeof(DeptGroup*));
                    
                    for (int j = 0; j < M; j++) {
                        DeptGroup* rehash_curr = table[j];
                        while (rehash_curr != NULL) {
                            DeptGroup* next_node = rehash_curr->next;
                            int new_h = hashDept(rehash_curr->dept, new_M);
                            
                            // Re-link the entire group into the new table
                            rehash_curr->next = new_table[new_h];
                            new_table[new_h] = rehash_curr;
                            
                            rehash_curr = next_node;
                        }
                    }
                    free(table);
                    table = new_table;
                    M = new_M;
                }
            }
        } 
        else if (strcmp(op, "DELETE") == 0) {
            if (group != NULL) {
                int idx = findPos(group->rolls, group->size, rollNo);
                if (idx < group->size && group->rolls[idx] == rollNo) {
                    // Shift elements left to close the gap
                    for (int j = idx; j < group->size - 1; j++) {
                        group->rolls[j] = group->rolls[j + 1];
                    }
                    group->size--;
                    total_records--;

                    // If group is empty, free it and remove it from the chain
                    if (group->size == 0) {
                        if (group_prev != NULL) {
                            group_prev->next = group->next;
                        } else {
                            // It was the head of the bucket
                            table[h] = group->next;
                        }
                        free(group->rolls);
                        free(group);
                    }
                }
            }
        }
        else if (strcmp(op, "SEARCH") == 0) {
            if (group != NULL) {
                int idx = findPos(group->rolls, group->size, rollNo);
                if (idx < group->size && group->rolls[idx] == rollNo) {
                    printf("FOUND\n");
                } else {
                    printf("NOT FOUND\n");
                }
            } else {
                printf("NOT FOUND\n");
            }
        } 
        else if (strcmp(op, "RANK") == 0) {
            if (group != NULL) {
                int idx = findPos(group->rolls, group->size, rollNo);
                printf("%d\n", idx);
            }
        }
    }

    // Free memory before exit
    for (int i = 0; i < M; i++) {
        DeptGroup* curr = table[i];
        while (curr != NULL) {
            DeptGroup* temp = curr;
            curr = curr->next;
            free(temp->rolls);
            free(temp);
        }
    }
    free(table);

    return 0;
}