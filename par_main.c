#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>

#define N 32

#define MASTER 1

#define EVERYBODY_GO -7

#define WORLD_SIZE 2


struct timeval t1, t2;

void start_timer();
void stop_timer();
void print_timer();
int getID();

int main()
{
    // File descriptor
    int fd;
    int* a;
    int myID, i, j, k;
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
    if ((a = mmap(NULL, (N + 2) * 4, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

    if(myID == MASTER)
    {
        // It is used to determine others working status
        a[N + 1] = EVERYBODY_GO;
        a[N] = -1 * WORLD_SIZE;
    }
    else
    {
        while(a[N + 1] != EVERYBODY_GO); // Wait for master's signal.
        printf("Got the signal, pleas wait...\n");
    }

    // This is very expensive (not that expensive though...)
    for(i = 0; i <= N - 1; i += WORLD_SIZE)
    {
        a[i + myID - 1] = 0;
        for(j = 0; j <= N - 1; j++)
        {
            for(k = 0; k <= N - 1; k++)
            {
                // Do something
                a[i + myID - 1] = j * k;
            }
        }
    }

    // It says: My job is finished!
    a[N] = a[N] + 1;

    if(myID == MASTER)
    {
        printf("Waiting for my pals to finish their job!\n");
        while(a[N] != 0); // wait for others

        for(i = 0; i <= N; i++)
        {
            printf("a[%d] = %d\n", i, a[i]);
            if(i%10 == 0)
                getchar();
        }
    }

    // Unmapping memory
    munmap(a, (N + 2) * 4);
    if(myID == MASTER)
    {
        // Remember: this memory is persistent! So always clear this flag...
        a[N + 1] = 0;
        stop_timer();
        print_timer();
    }
    return 0;
}

int getID()
{
    char a[8];
    char ids[3];
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
