/*
 * TIME AND SPACE COMPLEXITY:
 * - Part 1 (Stack): Time O(n), Space O(n) for the stack.
 * - Part 2 (Bonus): Time O(n), Space O(1) auxiliary space (no stack used).
 * - Overall Program: Time O(n), Space O(n) total.
 * 
 * MEMORY LEAK TESTING:
 * Tested locally using Valgrind.
 * Command used: valgrind --leak-check=full ./lab1
 * Output confirmed: "All heap blocks were freed -- no leaks are possible."
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// --- Linked List Setup ---
typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) exit(1);
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

void freeList(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// In-place list reversal (Needed for Bonus)
Node* reverseList(Node* head) {
    Node* prev = NULL;
    Node* current = head;
    Node* next = NULL;
    
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

// --- Array-based Stack Setup (Needed for Standard) ---
typedef struct {
    int* elements;
    int top;
    int capacity;
} Stack;

void initStack(Stack* s, int capacity) {
    s->capacity = capacity;
    s->top = -1;
    s->elements = (int*)malloc(capacity * sizeof(int));
    if (!s->elements) exit(1);
}

void push(Stack* s, int value) {
    s->elements[++(s->top)] = value;
}

int pop(Stack* s) {
    return s->elements[(s->top)--];
}

void freeStack(Stack* s) {
    free(s->elements);
}


// --- Main Pipeline ---
int main() {
    int n;
    printf("Enter the number of elements (n): ");
    fflush(stdout);
    if (scanf("%d", &n) != 1) return 1;
    
    if (n == 0) {
        printf("PALINDROME\n");
        return 0;
    }

    // 1. Read n integers into an array
    int* arr = (int*)malloc(n * sizeof(int));
    if (!arr) return 1;
    printf("Enter %d integers separated by spaces: ", n);
    fflush(stdout);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // 2. Build a singly linked list from the array
    Node* head = createNode(arr[0]);
    Node* current = head;
    for (int i = 1; i < n; i++) {
        current->next = createNode(arr[i]);
        current = current->next;
    }
    free(arr); // Free array as soon as we don't need it

    // Handle n=1 edge case early
    if (n == 1) {
        printf("PALINDROME\n");
        freeList(head);
        return 0;
    }

    bool isPalindrome = true;

    // ==========================================
    // PART 1: STANDARD REQUIRED METHOD (STACK)
    // ==========================================
    Stack s;
    initStack(&s, n);
    
    current = head;
    while (current != NULL) {
        push(&s, current->data);
        current = current->next;
    }

    current = head;
    while (current != NULL) {
        int poppedValue = pop(&s);
        if (current->data != poppedValue) {
            isPalindrome = false;
            break; // Note: We still free memory below even if we break
        }
        current = current->next;
    }
    freeStack(&s); // Stack no longer needed

    // ==========================================
    // PART 2: BONUS METHOD (O(1) EXTRA SPACE)
    // ==========================================
    // We execute this to prove the bonus works.
    
    Node* slow = head;
    Node* fast = head->next;
    
    // Find midpoint
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Reverse the second half
    Node* secondHalfHead = reverseList(slow->next);
    
    // Compare both halves
    Node* p1 = head;
    Node* p2 = secondHalfHead;
    bool isPalindromeBonus = true;
    
    while (p2 != NULL) {
        if (p1->data != p2->data) {
            isPalindromeBonus = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    // Restore the original list structure
    slow->next = reverseList(secondHalfHead);

    // ==========================================
    // FINAL OUTPUT & CLEANUP
    // ==========================================
    // Both methods should agree. We print the result.
    if (isPalindrome && isPalindromeBonus) {
        printf("PALINDROME\n");
    } else {
        printf("NOT A PALINDROME\n");
    }

    // 6. Free the linked list (no leaks)
    freeList(head);

    return 0;
}