#ifndef BEAM_UTILS_H
#define BEAM_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

// split a string into an array of strings
// internally this function uses strtok_r

typedef struct split_r
{
    char **tokens;
    size_t count;
} split_r;

void free_split(split_r *split)
{
    if (split == NULL)
        return;

    if (split->tokens != NULL)
    {
        for (int i = 0; i < split->count; i++)
        {
            free(split->tokens[i]);
        }
        free(split->tokens);
    }
    free(split);
}

split_r *split(char *str, const char *delim, int max_tokens)
{
    if (max_tokens <= 0 || strlen(str) == 0 || delim == NULL)
    {
        errno = EINVAL;
        return NULL;
    }

    split_r *result = (split_r *)malloc(sizeof(split_r));

    if (result == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for split_r");
        return NULL;
    }

    char **tokens = (char **)malloc(sizeof(char *) * max_tokens);
    if (tokens == NULL)
    {
        errno = ENOMEM;
        perror("could not allocate memory for tokens");
        free(result);
        return NULL;
    }

    result->tokens = tokens;
    result->count = 0;

    char *dupe = strdup(str);
    if (dupe == NULL)
    {
        perror("could not duplicate string");
        free(result);
        free(tokens);
        return NULL;
    }

    char *save_ptr;
    char *token = strtok_r(dupe, delim, &save_ptr);

    while (token != NULL && result->count < max_tokens)
    {
        tokens[result->count] = strdup(token);
        if (tokens[result->count] == NULL)
        {
            free(dupe);
            free_split(result);
            return NULL;
        }
        result->count++;
        token = strtok_r(NULL, delim, &save_ptr);
    }

    free(dupe);
    return result;
}

// Binary trees - In binary trees each node can have at most two children.
typedef struct TreeNode
{
    int data;
    TreeNode *left, *right;
} TreeNode;

TreeNode *createNode(int data)
{
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
}

typedef struct QueueNode
{
    int data;
    QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front, *rear;
} Queue;

QueueNode *createQueueNode(int data)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    if (node == NULL)
    {
        perror("could not allocate memory for QueueNode");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Queue *createQueue(int data)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL)
    {
        perror("could not allocate memory for Queue");
        exit(EXIT_FAILURE);
    }
    queue->front = createQueueNode(data);
    queue->rear = queue->front;
    return queue;
}

void enqueue(Queue *queue, int data)
{
    QueueNode *node = createQueueNode(data);
    if (queue->front == NULL)
    {
        queue->front = queue->rear = node;
        return;
    }
    queue->rear->next = node;
    queue->rear = node;
}

QueueNode *dequeue(Queue *queue)
{
    if (queue->front == NULL)
    {
        return NULL;
    }
    QueueNode *node = queue->front;
    queue->front = queue->front->next;

    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }

    return node;
}

typedef struct BSTNode
{
    int data;
    BSTNode *left, *right;
} BSTNode;

BSTNode *createBSTNode(int data)
{
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
}

// void insertBST(BSTNode *root, int data)
// {
//     if (root == NULL)
//     {
//         root = createBSTNode(data);
//         return;
//     }
//     if (data < root->data)
//     {
//         insertBST(root->left, data);
//     }
//     else if (data > root->data)
//     {
//         insertBST(root->right, data);
//     }
// }

#endif
