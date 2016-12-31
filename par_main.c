#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>

#define PROBLEM_SIZE 65536

#define MASTER 1

#define GO_EVERYBODY 7

#define WORLD_SIZE 2


struct timeval t1, t2;

void start_timer();
void stop_timer();
void print_timer();
void doSort(int[], int, int);
int getID();
void mergeSortedParts(int[], int);

int main()
{
    // File descriptor
    int fd;
    int* array;
    int myID, i, myStartingPosition, myEndPosition;
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
    // PROBLEM_SIZE + 1: One for end_job_flag. Integers reserve 4 bytes.
    if ((array = mmap(NULL, (PROBLEM_SIZE + 1) * 4, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

    if(myID == MASTER)
    {
        // It is used to determine others working status
        array[PROBLEM_SIZE] = -1 * WORLD_SIZE;
    }
    else
    {
        while(array[PROBLEM_SIZE] != -1 * WORLD_SIZE); // Wait for master's signal.
        printf("Got the signal, pleas wait...\n");
    }

    myStartingPosition = (myID - 1) * ((PROBLEM_SIZE - 1) / WORLD_SIZE);
    myEndPosition = myStartingPosition + ((PROBLEM_SIZE - 1) / WORLD_SIZE) - 1;

    for(i = myStartingPosition; i <= myEndPosition; i++)
    {
        // storing the array in desc order: this is worst case scenario for sorting
        array[i] = PROBLEM_SIZE - 1 - i;
    }

    doSort(array, myStartingPosition, myEndPosition);

    // It says: My job is finished!
    array[PROBLEM_SIZE]++;

    if(myID == MASTER)
    {
        printf("Waiting for my pals to finish their job!\n");
        while(array[PROBLEM_SIZE] != 0); // wait for others
        // Array is partialy sorted, so we have to merge them.
        mergeSortedParts(array, myEndPosition);
                for(i = 65500; i <= PROBLEM_SIZE; i++)
                {
            printf("%d\n", array[i]);
            if(i%5 == 0)
            getchar();
            }
            }

    // Unmapping memory
    munmap(array, (PROBLEM_SIZE + 1) * 4);
    if(myID == MASTER)
    {
        stop_timer();
        print_timer();
    }
    return 0;
}

void mergeSortedParts(int array[], int myEndPosition)
{
    int i, j;

    for(i = 0; i <= myEndPosition; i++)
    {
        for(j = i + myEndPosition + 1; j <= PROBLEM_SIZE - 1; j+=myEndPosition)
        {
            if(array[i] > array[j])
            {
                // swapping values (note: no temp variable used)
                array[i] = array[i] + array[j];
                array[j] = array[i] - array[j];
                array[i] = array[i] - array[j];
            }
        }
    }
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

// This is the expensive function
void doSort(int array[], int myStartingPosition, int myEndPosition)
{
    int i, j;

    for(i = myStartingPosition; i <= myEndPosition; i++)
    {
        for(j = i + 1; j <= myEndPosition; j++)
        {
            if(array[i] > array[j])
            {
                // swapping values (note: no temp variable used)
                array[i] = array[i] + array[j];
                array[j] = array[i] - array[j];
                array[i] = array[i] - array[j];
            }
        }
    }
}
