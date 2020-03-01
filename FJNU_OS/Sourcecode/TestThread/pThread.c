#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_THREADS 8

void * perform(void * argu)
{
    int value_of_argu;

    value_of_argu = *((int *)argu);
    printf("***trd%d*** Hello, world!\n", value_of_argu);

    /* insert more useful stuff here */

    return NULL;
}

int main(int argc, char ** argv)
{
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    int value_of_result = 0;
    int index = 0;

    for (index = 0; index < NUM_THREADS; index++)
    {
        thread_args[index] = index;
        printf("***main*** Process creates thread %d.\n", index);
        value_of_result =
            pthread_create(&threads[index], NULL,
                           perform, &thread_args[index]);
        assert(!value_of_result);
    }

    for (index = 0; index < NUM_THREADS; index++)
    {
        value_of_result =
            pthread_join(threads[index], NULL);
        assert(!value_of_result);
        printf("***main*** Thread %d exited.\n", index);
    }

    printf("***main*** All threads has completed successfully.\n");

    return 0;
}
