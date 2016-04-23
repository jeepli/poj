#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEX (100)

unsigned int max_uint(unsigned int n1, unsigned int n2)
{
    if (n1 > n2) 
    {
        return n1;
    }
    return n2;
}

typedef struct bignum
{
    unsigned int size;
    unsigned int index;
    char *pvalue;   
}BIG_NUM_S;


int bignum_init(BIG_NUM_S *num, unsigned int size)
{
    int ret = -1;
    char *p = (char *) malloc(size);
    if (NULL != p)
    {
        memset(p, 0, size);
        num->index = 0;
        num->pvalue = p;
        num->size = size;
        ret = 0;
    }
    
    return ret; 
}

void bignum_destroy(BIG_NUM_S *num)
{
    if (NULL != num->pvalue)
    {
        free(num->pvalue);
        num->pvalue = NULL;
    }
}

void bignum_clear(BIG_NUM_S *num)
{
    if (NULL != num->pvalue)
    {
        memset(num->pvalue, 0, num->size);
    }
    num->index = 0;
}

int bignum_resize(BIG_NUM_S *num, unsigned int size)
{
    char *p = NULL;
    if (size <= num->index)
    {
        return -1;
    }

    p = (char *) malloc(size);
    if (NULL != p)
    {
        memset(p, 0, size);
        if (NULL != num->pvalue)
        {
            memcpy(p, num->pvalue, (num->index + 1));
            free(num->pvalue);
        }
        num->size = size;   
        num->pvalue = p;
        return 0;
    }
   
    return -1;
}

void bignum_copy(BIG_NUM_S *to, BIG_NUM_S *from)
{
    bignum_clear(to);
    bignum_resize(to, from->size);
    to->index = from->index;
    memcpy(to->pvalue, from->pvalue, (from->index + 1));
}


void bignum_str(BIG_NUM_S *num, char *str, unsigned int len)
{
    unsigned int printlen = 0;
    int index;
    if (NULL == str) return;
    
    index = num->index;
    printlen += snprintf(str, len, "%d", (unsigned int) num->pvalue[index]);
    
    for (index = num->index - 1; index >= 0 && printlen < len; index--)
    {
        printlen += snprintf(str + printlen, (len - printlen), "%02d", (unsigned int) (num->pvalue[index]));
    }
}

void bignum_debug_info(BIG_NUM_S *num)
{
    char str[1024] = {0};
    bignum_str(num, str, 1024);
    printf("index:%d,size%d,value:%s\n", num->index, num->size, str);       
}

void bignum_from_uint(BIG_NUM_S *num, unsigned int n)
{
    unsigned int size = 1;
    unsigned int temp = n/HEX;
    unsigned int i;
    char c;
    while(temp > 0)
    {
        size++;
        temp /= HEX;
    }

    if (size < num->size)
    {
        bignum_clear(num);
        bignum_resize(num, size);
    }

    temp = n;
    for (i = 0; i < size; i++)
    {
        c = (char) (temp%HEX);
        num->pvalue[i] = c;
        temp /= HEX;
    }
    
    num->index = size -1;
}

void bignum_add(BIG_NUM_S *n1, BIG_NUM_S *n2, BIG_NUM_S *r)
{
    unsigned int need_size;
    unsigned int temp;
    unsigned int shift = 0;
    unsigned int i;

    need_size = n1->index + n2->index + 2 + 1;
    bignum_clear(r);
    bignum_resize(r, need_size);

    for (i = 0; i <= n1->index && i <= n2->index; i++)
    {
        temp = (unsigned int)n1->pvalue[i] + (unsigned int) n2->pvalue[i] + shift;
        shift = temp/HEX;
        r->pvalue[i] = (char) (temp%HEX);
    }

    if (i > n1->index)
    {
        for (; i <= n2->index; i++)
        {

            temp = (unsigned int) n2->pvalue[i] + shift;
            shift = temp/HEX;
            r->pvalue[i] = (char) (temp%HEX);
        }
    }
    else
    {
        for (; i <= n1->index; i++)
        {

            temp = (unsigned int) n1->pvalue[i] + shift;
            shift = temp/HEX;
            r->pvalue[i] = (char) (temp%HEX);
        }
    }

    if (shift > 0)
    {
        r->index = i;
        r->pvalue[i] = (char) shift;
    }
    else
    {
        r->index = (i == 0) ? 0: (i - 1);
    }
}

void bignum_multi(BIG_NUM_S *n1, BIG_NUM_S *n2, BIG_NUM_S *r)
{
    unsigned int need_size;
    int i;
    int j;
    int k;
    unsigned int temp;
    need_size = n1->index + n2->index + 2 + 2;
    bignum_clear(r);
    bignum_resize(r, need_size);

    for (i = 0; i <= n1->index; i++)
    {
        unsigned int shift = 0;
        BIG_NUM_S nt;
        BIG_NUM_S nr;
        bignum_init(&nt, need_size);
        bignum_init(&nr, 1);
        bignum_copy(&nr, r);
        
        for (j = 0; j <= n2->index; j++)
        {
            temp = n1->pvalue[i] * n2->pvalue[j] + shift;
            shift = (temp / HEX);
            nt.pvalue[i + j] = (char) (temp % HEX);
        }
        
        if (shift > 0)
        {
            nt.pvalue[i + j] = (char) shift;
        }

        for (k = need_size - 1; k > 0; k--)
        {
            if (nt.pvalue[k] != 0) break;
        }
        nt.index = k;
        //printf("r is:");
        //bignum_debug_info(r);
        //printf("nt is:");
        //bignum_debug_info(&nt);

        bignum_add(&nt, &nr, r);
    } 
}


void f_to_i(char *s, int *n)
{
    int i = 0;
    for (i = 0; i < 6; i++)
    {
        if (s[i] == '.') break;
    }
    
    if (i == 6) 
    {
        n = 0;
        return;
    }

    *n = 6 - i - 1;
    for (i = 5; i >= 0; i--)
    {
        if (s[i] != '0') break;
        (*n) = (*n) - 1;
    }
}


void expon(unsigned int num, int p, BIG_NUM_S *r)
{
    int i;
    BIG_NUM_S n;
    bignum_init(&n,1);
    bignum_from_uint(&n,num);
    bignum_clear(r);
    bignum_from_uint(r, 1);
    
    for (i = 1; i <= p; i++)
    {
        BIG_NUM_S tr;
        bignum_init(&tr, 1);
        bignum_copy(&tr, r);
        bignum_multi(&n, &tr, r);
    }
}

void result_print(char *r, int len, int n)
{
    int i = 0;
    if (n >= len)
    {
        printf(".");
        for (i = 0; i < n -len; i++)
        {
            printf("0");
        }
        printf("%s\n", r);
    }
    else
    {
        for (i = 0; i < len - n; i++)
        {
            printf("%c", r[i]);
        }
        printf(".%s\n", r + i);
    }
}

void result_trnas(char *r, int len, int n, char *r2)
{
    int i = 0;
    int j = 0;
    if (n >= len)
    {
        r2[j++] = '.';
        for (i = 0; i < n -len; i++)
        {
            r2[j++] = '0'; 
        }
        memcpy(r2 + j, r, len);
    }
    else
    {
        for (i = 0; i < len - n; i++)
        {
            r2[j++] = r[i];
        }
        if (n != 0)
        {
            r2[j++] = '.';
            memcpy(r2 + j, r + i, len - i);
        }
    }
}


int main(int argc, char **argv)
{
    char line[10] = {0};
    char results[100][1024] = {0};
    int rindex = -1;
    int p;
    int n;
    int in;
    float dn;
    unsigned int i;
    //scanf("%s", line);
    //scanf("%d", &p);
    while(EOF != scanf("%s %d", line, &p))
    {
        f_to_i(line,&n);
    
        dn = atof(line);
        for (i = 0; i < n; i++)
        {
            dn = dn * (10.0);
        }
        in = (unsigned int) (int) dn;

        BIG_NUM_S r;
        bignum_init(&r, 1);
        expon(in, p, &r);
        //bignum_debug_info(&r);
        char result[1024] = {0};
    
        bignum_str(&r, result, 1024);
        rindex++;
        result_trnas(result, strlen(result), n * p, results[rindex]);
        //result_print(result, strlen(result), n * p);
    } 

    for (i = 0; i <= rindex; i++)
    {
        printf("%s\n", results[i]);
    }

    
    return 0;
}

