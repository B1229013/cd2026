#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp;
    int count[256];
    int c;
    int i;

    for(i = 0; i < 256; i = i + 1)
    {
        count[i] = 0;
    }

    fp = fopen("main.c", "r");

    if(fp == NULL)
    {
        printf("Cannot open file\n");
        return 0;
    }

    while((c = fgetc(fp)) != EOF)
    {
        count[(unsigned char)c] = count[(unsigned char)c] + 1;
    }

    fclose(fp);

    for(i = 0; i < 256; i = i + 1)
    {
        if(count[i] > 0)
        {
            printf("%c : %d\n", i, count[i]);
        }
    }

    return 0;
}
