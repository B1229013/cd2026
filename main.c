#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char ch;
    int count;
    struct Node *next;
} Node;

Node* find(Node *head, char c)
{
    Node *curr = head;

    while(curr != NULL)
    {
        if(curr->ch == c)
        {
            return curr;
        }

        curr = curr->next;
    }

    return NULL;
}

int main()
{
    FILE *fp;
    Node *head = NULL;
    Node *curr;
    Node *newNode;
    char c;

    fp = fopen("main.c", "r");

    if(fp == NULL)
    {
        printf("Cannot open file\n");
        return 0;
    }

    while((c = fgetc(fp)) != EOF)
    {
        Node *node = find(head, c);

        if(node != NULL)
        {
            node->count = node->count + 1;
        }
        else
        {
            newNode = (Node*)malloc(sizeof(Node));

            newNode->ch = c;
            newNode->count = 1;
            newNode->next = head;

            head = newNode;
        }
    }

    fclose(fp);

    curr = head;

    while(curr != NULL)
    {
        printf("%c : %d\n", curr->ch, curr->count);
        curr = curr->next;
    }

    return 0;
}
