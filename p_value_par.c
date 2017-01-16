#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#define MASTER 1

#define EVERYBODY_GO -7

#define WORLD_SIZE 2

#define N 100

struct timeval t1, t2;

struct point
{
    float x;
    float y;
};

struct point points[N / 2];

int M;
int myID;


int getID();
void start_timer();
void stop_timer();
void print_timer();
void init_points(int);
float get_random();
void print_points();
int evaluate_M(int);
void print_p_value();

int main()
{
    // File descriptor
    int fd;
    int* a;
    int i, j, k;
    char input;
    myID = getID();
    if(myID == MASTER)
    {
        printf("Press Y to start...\n");
        do
        {
            scanf("%c", &input);
        }
        while(input != 'Y' && input != 'y');
        printf("Please wait...\n");
        start_timer();
    }
    else
    {
        printf("Waiting for boss' permission!\n");
    }
    if ((fd = open("/mnt/fabric_emulation", O_RDWR)) == -1)
    {
        perror("open failed");
        exit(1);
    }
    // PROBLEM_SIZE + 1: One for start_job_flag, the other for end_job_flag.
    if ((a = mmap(NULL, 3 * sizeof(int), PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

    // Remember: this memory is persistent! So always clear these...
    a[2] = 0;
    a[1] = 0;
    a[0] = 0;

    if(myID == MASTER)
    {
        // It is used to determine others working status
        a[2] = EVERYBODY_GO;
        a[1] = -1 * WORLD_SIZE;
    }
    else
    {
        while(a[2] != EVERYBODY_GO); // Wait for master's signal.
        printf("Got the signal, pleas wait...\n");
    }

    // This is very expensive (not that expensive though...)

    srand(time(NULL));
    init_points();
    M = evaluate_M();

    a[0] += M;

    // It says: My job is finished!
    a[1] = a[1] + 1;

    if(myID == MASTER)
    {
        printf("Waiting for my pals to finish their job!\n");
        while(a[1] != 0); // wait for others
        M = a[0];
    }

    // Unmapping memory
    munmap(a, 3 * 4);
    if(myID == MASTER)
    {
        print_p_value();
        stop_timer();
        print_timer();
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
    float elapsedTime;
    // sec to ms
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;
    // us to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;
    printf("Elapsed time := %f ms.\n", elapsedTime);
}

void init_points(int n)
{
    int i, j;
    for(i=0; i<=n-1; i+=WORLD_SIZE)
    {
        for(j=i; j< i + WORLD_SIZE; j++)
        {
            if(j - i + 1 == myID)
            {
                points[j].x = get_random();
                points[j].y = get_random();
            }
        }
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

int evaluate_M(int n)
{
    int i, j;
    for(i=0; i<=n-1; i+=WORLD_SIZE)
    {
        for(j=i; j< i + WORLD_SIZE; j++)
        {
            if(j - i + 1 == myID)
            {
                if(points[j].x * points[j].x + points[j].y * points[j].y < 1)
                    M++;
            }
        }
    }
}

void print_p_value()
{
    printf("Estimated P value := %1.3f.\n", 4.0 * M / N);
}

int getID()
{
    char a[8];
    char ids[3];
    printf("dDDDDDDDDDDDDDDDDDDDDDDDddddddddddddddddddddddD");
    gethostname(a, 8);
    strcat(a, "$");
    ids[0] = a[6];
    if(a[7] != '$')
    {
        ids[1] = a[7];
        ids[2] = '\0';
    }
    else
    {
        ids[1] = '\0';
    }
    return atoi(ids);
}
