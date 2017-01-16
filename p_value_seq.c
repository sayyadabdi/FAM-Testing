#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define N 10000000

struct point
{
    float x;
    float y;
};

struct timeval t1, t2;
struct point points[N];

int M;
int i;

void start_timer();
void stop_timer();
void print_timer();
void init_points();
float get_random();
void print_points();
void evaluate_M();
void print_p_value();

int main()
{
    start_timer();
    i = 1;
    while(i <= 10)
    {
        // Start Job
        srand(time(NULL));
        init_points();
        evaluate_M();
        print_p_value();
        // End Job
        i++;
    }
    stop_timer();
    print_timer();
    return EXIT_SUCCESS;
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
    float elapsedTime;
    // sec to ms
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    // us to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("Elapsed time := %f ms.\n", elapsedTime);
}

void init_points()
{
    int i;
    for(i=0; i<=N-1; i++)
    {
        points[i].x = get_random();
        points[i].y = get_random();
    }
}

float get_random()
{
    return -1.0 + ((float)rand() / (float)RAND_MAX * 2.0);
}

void print_points()
{
    int i;
    for(i=0; i<=N-1; i++)
    {
        printf("Point[%i]: x = %f, y = %f\n", i, points[i].x, points[i].y);
    }
}

void evaluate_M()
{
    int i;
    for(i=0; i<=N-1; i++)
    {
        if(points[i].x * points[i].x + points[i].y * points[i].y < 1)
            M++;
    }
}

void print_p_value()
{
    printf("Estimated P value := %1.3f.\n", 4.0 * M / N);
}
