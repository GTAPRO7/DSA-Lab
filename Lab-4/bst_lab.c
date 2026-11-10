#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

// Define the BST Node structure
typedef struct Node {
    int key;
    struct Node* left;
    struct Node* right;
} Node;

// PART A: Build the Tree

// A1. Allocate a new node with malloc
Node* createNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// A2. Recursive insertion (ignores duplicates)
Node* insert(Node* root, int key) {
    // Base case: found an empty spot, create the node here
    if (root == NULL) {
        return createNode(key);
    }
    
    // Otherwise, recur down the tree
    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    }
    // Note: If key == root->key, we do nothing (duplicates are ignored)
    
    return root;
}

// A3. Iterative search (returns 1 if found, 0 if not)
int search(Node* root, int key) {
    Node* current = root;
    while (current != NULL) {
        if (key == current->key) {
            return 1; // Found the key
        } else if (key < current->key) {
            current = current->left; // Go to left subtree
        } else {
            current = current->right; // Go to right subtree
        }
    }
    return 0; // Reached NULL, key is not in the tree
}

// PART B: Traversals and Measurements

// B1. Depth-First Traversals
void inorder(Node* root) {
    if (root == NULL) return;
    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

void preorder(Node* root) {
    if (root == NULL) return;
    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

void postorder(Node* root) {
    if (root == NULL) return;
    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->key);
}

// B3. Height of tree (Convention: empty tree = -1, single node = 0)
int height(Node* root) {
    if (root == NULL) {
        return -1;
    }
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

// B4. Count total nodes and leaf nodes
int countNodes(Node* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(Node* root) {
    if (root == NULL) return 0;
    // If both children are NULL, this node is a leaf
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    return countLeaves(root->left) + countLeaves(root->right);
}

// B5. Find Minimum and Maximum without recursion (Iterative)
Node* findMin(Node* root) {
    if (root == NULL) return NULL;
    // The smallest value is always the leftmost node
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Node* findMax(Node* root) {
    if (root == NULL) return NULL;
    // The largest value is always the rightmost node
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

// PART C: Deletion and Memory Cleanup

Node* deleteNode(Node* root, int key) {
    if (root == NULL) return NULL;

    // Step 1: Locate the node to be deleted
    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    } else {
        // We found the node, Handle the 3 deletion cases explicitly:

        // Case 1: Leaf node (no children)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2a: One child (only right child exists)
        else if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        // Case 2b: One child (only left child exists)
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        // Case 3: Two children
        else {
            // Find inorder successor (smallest node in the right subtree)
            Node* successor = findMin(root->right);
            // Copy the successor's key into this current node
            root->key = successor->key;
            // Recursively delete the successor from the right subtree
            root->right = deleteNode(root->right, successor->key);
        }
    }
    return root;
}

// Free all nodes using postorder traversal (children freed before parent)
void freeTree(Node* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

// PART D: Why Balance Matters

// D1. Verify BST property across the entire tree using valid [min, max] range
int isBST(Node* root, int min, int max) {
    if (root == NULL) return 1;
    
    // If this node's key falls outside the valid range, it violates the BST rule
    if (root->key <= min || root->key >= max) {
        return 0;
    }
    
    // Check left subtree (max allowed is root->key) and right subtree (min allowed is root->key)
    return isBST(root->left, min, root->key) && 
           isBST(root->right, root->key, max);
}

int main() {
    // --- PART A & B ---
    Node* root = NULL;
    int keys[] = {50, 30, 70, 20, 40, 60, 80, 35};
    int n = 8;

    for (int i = 0; i < n; i++) {
        root = insert(root, keys[i]);
    }

    printf("Inorder: "); inorder(root); printf("\n");
    printf("Preorder: "); preorder(root); printf("\n");
    printf("Postorder: "); postorder(root); printf("\n");

    printf("Height: %d Nodes: %d Leaves: %d\n", 
           height(root), countNodes(root), countLeaves(root));

    printf("Search 60: %s ", search(root, 60) ? "found" : "not found");
    printf("Search 25: %s ", search(root, 25) ? "found" : "not found");
    printf("Search 90: %s\n", search(root, 90) ? "found" : "not found");

    Node* minNode = findMin(root);
    Node* maxNode = findMax(root);
    printf("Min: %d Max: %d\n\n", minNode->key, maxNode->key);

    // --- PART C: DELETION ---
    printf("--- Part C: Deletions ---\n");
    
    // 1. Delete 35 (Leaf node case)
    root = deleteNode(root, 35);
    printf("Inorder after deleting 35 (Leaf): ");
    inorder(root); printf("\n");
    root = insert(root, 35); 
    
    // 2. Delete 40 (One child case - 40 has left child 35)
    root = deleteNode(root, 40);
    printf("Inorder after deleting 40 (One child): ");
    inorder(root); printf("\n");

    // 3. Delete 30 (Two children case - 30 has left child 20 and right child 35)
    root = deleteNode(root, 30);
    printf("Inorder after deleting 30 (Two children): ");
    inorder(root); printf("\n\n");

    // --- PART D: BALANCE & VALIDATION ---
    printf("--- Part D: Validation & Balance ---\n");
    printf("Is our main tree a valid BST? %s\n", 
           isBST(root, INT_MIN, INT_MAX) ? "YES" : "NO");

    // Build the invalid tree from D1 manually
    Node* invalidTree = createNode(50);
    invalidTree->left = createNode(30);
    invalidTree->right = createNode(70);
    invalidTree->left->left = createNode(20);
    invalidTree->left->right = createNode(55); // Invalid! 55 > 50, sitting in 50's left subtree

    printf("Is the manual tree (with 55) a valid BST? %s\n", 
           isBST(invalidTree, INT_MIN, INT_MAX) ? "YES" : "NO");
    freeTree(invalidTree);

    // D2: Height of 1 to 15 in sorted order
    Node* sortedTree = NULL;
    for (int i = 1; i <= 15; i++) {
        sortedTree = insert(sortedTree, i);
    }
    printf("Height of 1..15 (Sorted order): %d\n", height(sortedTree));
    freeTree(sortedTree);

    // D2: Height of 1 to 15 in balanced order
    Node* balancedTree = NULL;
    int balKeys[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15};
    for (int i = 0; i < 15; i++) {
        balancedTree = insert(balancedTree, balKeys[i]);
    }
    printf("Height of 1..15 (Balanced order): %d\n", height(balancedTree));
    freeTree(balancedTree);

    // D3: Extension - 1000 random integers
    Node* randomTree = NULL;
    srand(time(NULL));
    for (int i = 0; i < 1000; i++) {
        randomTree = insert(randomTree, rand() % 100000);
    }
    printf("Height of 1000 random integers: %d (log2(1000) is ~9.97)\n", height(randomTree));
    freeTree(randomTree);

    // Free the main tree before exiting
    freeTree(root);

    return 0;
}