#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

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

int main()
{
    // File descriptor
    int fd;
    int* array;
    int myID, i, myStartingPosition, myEndPosition;
    char input;
    printf("Press Y to start...\n");
    do
    {
        scanf("%c", &input);
    }
    while(input != 'Y' && input != 'y');
    start_timer();
    printf("Please wait...\n");
    myID = getID();
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

    // It is used to determine others working status
    array[PROBLEM_SIZE] = 0;

    myStartingPosition = (myID - 1) * ((PROBLEM_SIZE - 1) / WORLD_SIZE);
    myEndPosition = myStartingPosition + ((PROBLEM_SIZE - 1) / WORLD_SIZE) - 1;

    for(i = myStartingPosition; i <= myEndPosition; i++)
    {
        // storing the array in desc order: this is worst case scenario for sorting
        array[i] = len - 1 - i;
    }

    doSort(array, myStartingPosition, myEndPosition);

    // It says: My job is finished!
    array[PROBLEM_SIZE]++;

    if(myID == MASTER)
    {
        while(array[PROBLEM_SIZE] != WORLD_SIZE); // wait for others
    }

    // Unmapping memory
    munmap(array, (PROBLEM_SIZE + 1) * 4);
    stop_timer();
    print_timer();
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
