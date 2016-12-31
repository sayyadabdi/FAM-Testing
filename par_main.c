#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#define PROBLEM_SIZE 65536

#define MASTER 1


struct timeval t1, t2;

void start_timer();
void stop_timer();
void print_timer();
void doSort(int[], int);
int getID();

int main()
{
    int* array;
    int myID, i, len;
    char input;
    printf("Press Y to start...\n");
    do
    {
        scanf("%c", &input);
    }
    while(input != 'Y' && input != 'y');
    start_timer();
    myID = getID();

    if(myID == MASTER)
    {

    }
    printf("Please wait...\n");
    len = 65536;
    for(i = 0; i <= len - 1; i++)
    {
        // storing the array in desc order: this is worst case scenario for sorting
        array[i] = len - 1 - i;
    }
    doSort(array, len);
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
void doSort(int array[], int len)
{
    int i, j;

    for(i = 0; i <= len - 1; i++)
    {
        for(j = i + 1; j <= len - 1; j++)
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
