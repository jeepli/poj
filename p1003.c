#include <stdio.h>

double calc_weight(int n)
{
    int i;
    double r = 0.0;
    for (i = 1; i <= n; i++)
    {
       r += (1.0/ (double) (i + 1));
    }
    return r;
}

int main(int argc, char **argv)
{
    int n;
    double d[280];
    double input[280] = {0.0};
    double dt;
    int i,j,k;
    d[0] = 0.5;
    for (i = 1; i < 280; i++)
    {
        d[i] = d[i-1] + (1.0/(double) (i + 2));
    }    

    i = 0;
    do {
        scanf("%lf", &dt);
        input[i++] = dt;
    } while ( (dt - 0.000000001) > 0.0);

    for (j = 0; j < i - 1; j++)
    {
        for (k = 0; k < 280; k++)
        {
            if (d[k] - input[j] > -0.00000000000001)
            {
                printf("%d card(s)\n", k + 1);
                break;
            }
        }   
    }

    return 0;
}
