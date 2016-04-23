#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ch_to_int(char c)
{
    if ('0' <= c && '9' >= c) return (int) (c - '0');
    if ('A' <= c && 'C' >= c) return 2;
    if ('D' <= c && 'F' >= c) return 3;
    if ('G' <= c && 'I' >= c) return 4;
    if ('J' <= c && 'L' >= c) return 5;
    if ('M' <= c && 'O' >= c) return 6;
    if ('P' <= c && 'S' >= c) return 7;
    if ('T' <= c && 'V' >= c) return 8;
    if ('W' <= c && 'Y' >= c) return 9;
}

int str_to_int(char *s, int len)
{
    int i = 0;
    int r = 0;
    int temp;
    for (i = 0; i < len; i++)
    {
        if (('0' <= s[i] && '9' >= s[i]) || ('A' <= s[i] && 'Z' > s[i] && 'Q' != s[i]))
        {
            temp = ch_to_int(s[i]);
            r = r * 10 + temp;
        }   
    }
    return r;
}

int main(int argc, char **argv)
{
    int n;
    int i;
    char line[1024];
    int *result = (int *) malloc(10000000 * sizeof(int));
    memset(result, 0, 10000000 * sizeof(int));
    int temp;
    int flag = 0;
    scanf("%d", &n);
    for (i = 0; i < n; i++)
    {
        scanf("%s", line);
        temp = str_to_int(line, strlen(line));
        result[temp]++;
        memset(line, 0, 1024);       
    }
    
    for (i = 1; i < 10000000; i++)
    {
        if (result[i] > 1)
        {
            flag = 1;
            printf("%03d-%04d %d\n", (i/10000), (i % 10000), result[i]);
        }
    }    

    if (0 == flag)
    {
        printf("No duplicates.\n");
    }
    return 0;
}
