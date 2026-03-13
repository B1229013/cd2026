#include <stdio.h>
#include <stdlib.h>

struct Node
{
    char ch;
    struct Node *next;
};

int main()
{
    FILE *fp;
    int c;
    int printed[256] = {0};

    struct Node *head = NULL;
    struct Node *tail = NULL;
    struct Node *newNode;
    struct Node *temp;

    fp = fopen("main.c", "r");

    if(fp == NULL)
    {
        printf("Cannot open file\n");
        return 0;
    }

    while((c = fgetc(fp)) != EOF)
    {
        newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->ch = (char)c;
        newNode->next = NULL;

        if(head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }

    fclose(fp);

    temp = head;

    while(temp != NULL)
    {
        unsigned char ch = temp->ch;

        if(!printed[ch])
        {
            if(ch == '\n')
                printf("'\\n'\n");
            else if(ch == '\t')
                printf("'\\t' ");
            else if(ch == ' ')
                printf("' ' ");
            else
                printf("'%c' ", ch);

            printed[ch] = 1;
        }

        temp = temp->next;
    }

    printf("\n");

    return 0;
}
