#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define N 32

struct timeval t1, t2;

void start_timer();
void stop_timer();
void print_timer();

int main()
{
    double* a;
    int i, j, k;
    char input;
    printf("\nPress Y to start...\n");
    do
    {
        scanf("%c", &input);
    }
    while(input != 'Y' && input != 'y');
    start_timer();
    printf("Please wait...\n");

    a = malloc(N * sizeof(double));

    // This is very expensive (not that expensive though...)
    for(i = 0; i <= N - 1; i++)
    {
        a[i] = 0.0f;
        for(j = 0; j <= N - 1; j++)
        {
            for(k = 0; k <= N - 1; k++)
            {
                // Do something
                a[i] += sqrt(sqrt(a[i]) + sqrt(j * k));
            }
        }
    }
    stop_timer();
    print_timer();
    for(i = 0; i <= N - 1; i++)
    {
    printf("a[%f] = %f\n", i, a[i]);
    }
    return 0;
}

void start_timer()
{
    gettimeofday(&t1, NULL);
}

void stop_timer()
{
    gettimeofday(&t2, NULL);
}

void print_timer()
{
    int elapsedTime;
    elapsedTime = (t2.tv_sec - t1.tv_sec);
    printf("Elapsed time: %d seconds.\n", elapsedTime);
}
