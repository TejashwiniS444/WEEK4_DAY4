#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// A structure to represent a stack
struct Stack {
    int *array;
    int top;
    int capacity;
};

// Function to create a stack
struct Stack* createStack(int capacity) {
    struct Stack *stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*) malloc(stack->capacity * sizeof(int));
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack* stack) {
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is -1
int isEmpty(struct Stack* stack) {
    return stack->top == -1;
}

// Function to push an item to stack
void push(struct Stack* stack, int item) {
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

// Function to pop an item from stack
int pop(struct Stack* stack) {
    if (isEmpty(stack))
        return -1;
    return stack->array[stack->top--];
}

// A structure to represent an adjacency list node
struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

// A structure to represent an adjacency list
struct AdjList {
    struct AdjListNode *head;
};

// A structure to represent a graph
struct Graph {
    int V;
    struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V) {
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    // Initialize each adjacency list as empty by making head as NULL
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest) {
    // Add an edge from src to dest.  A new node is added to the adjacency
    // list of src. The node is added at the beginning
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

// A function to do DFS starting from vertex v
void dfs(struct Graph* graph, int v, bool visited[], struct Stack* stack) {
    visited[v] = true;

    // Recur for all vertices adjacent to this vertex
    struct AdjListNode* node = graph->array[v].head;
    while (node != NULL) {
        int dest = node->dest;
        if (!visited[dest])
            dfs(graph, dest, visited, stack);
        node = node->next;
    }

    // All vertices reachable from v are processed by now, push v to stack
    push(stack, v);
}

// Function to transpose the graph
struct Graph* transposeGraph(struct Graph* graph) {
    struct Graph* transposedGraph = createGraph(graph->V);
    for (int v = 0; v < graph->V; v++) {
        struct AdjListNode* node = graph->array[v].head;
        while (node != NULL) {
            addEdge(transposedGraph, node->dest, v);
            node = node->next;
        }
    }
    return transposedGraph;
}

// A utility function to print the strongly connected components
void printSCCs(struct Graph* graph, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);

    struct AdjListNode* node = graph->array[v].head;
    while (node != NULL) {
        int dest = node->dest;
        if (!visited[dest])
            printSCCs(graph, dest, visited);
        node = node->next;
    }
}

// The main function that finds and prints all strongly connected components
void findSCCs(struct Graph* graph) {
    struct Stack* stack = createStack(graph->V);

    // Mark all the vertices as not visited (for first DFS)
    bool* visited = (bool*) malloc(graph->V * sizeof(bool));
    for (int i = 0; i < graph->V; i++)
        visited[i] = false;

    // Fill vertices in stack according to their finishing times
    for (int i = 0; i < graph->V; i++)
        if (!visited[i])
            dfs(graph, i, visited, stack);

    // Create a transposed graph
    struct Graph* transposedGraph = transposeGraph(graph);

    // Mark all the vertices as not visited (for second DFS)
    for (int i = 0; i < graph->V; i++)
        visited[i] = false;

    // Now process all vertices in order defined by the stack
    while (!isEmpty(stack)) {
        int v = pop(stack);

        if (!visited[v]) {
            printSCCs(transposedGraph, v, visited);
            printf("\n");
        }
    }

    // Free memory
    free(visited);
    free(stack->array);
    free(stack);
}

int main() {
    // Create a graph
    int V = 5;
    struct Graph* graph = createGraph(V);
    addEdge(graph, 0, 2);
    addEdge(graph, 2, 1);
    addEdge(graph, 1, 0);
    addEdge(graph, 0, 3);
    addEdge(graph, 3, 4);

    printf("Strongly Connected Components:\n");
    findSCCs(graph);

    return 0;
}
