// Include the necessary libraries
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
// Include the necessary libraries


// Define the number of threads for each type
#define NUM_H1 8
#define NUM_H2 8
#define NUM_H3 8

// Define the mutexes for the files A and B
pthread_mutex_t mutex_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_B = PTHREAD_MUTEX_INITIALIZER;

// Define the condition variables for the files A and B
pthread_cond_t cond_A = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_B = PTHREAD_COND_INITIALIZER;

// Define the counters for the number of threads accessing the files A and B
int count_A = 0;
int count_B = 0;

// Define the function for H1 threads
void *h1(void *arg)
{
    // Get the thread number from the argument
    int num = *((int *)arg);

    // Get the thread id
    pthread_t id = pthread_self();

    // Lock the mutex for file A
    pthread_mutex_lock(&mutex_A);

    // Wait for the condition that no H3 thread is accessing file A
    while (count_A > 0)
    {
        pthread_cond_wait(&cond_A, &mutex_A);
    }

    // Increment the counter for file A
    count_A++;

    // Unlock the mutex for file A
    pthread_mutex_unlock(&mutex_A);

    // Print the message that the thread is accessing file A
    printf("Thread h1 number %d (id.%lu) accessing file A (%d %d)\n", num, id, count_A, count_B);

    // Simulate the use of file A
    sleep(rand() % 2);

    // Lock the mutex for file A
    pthread_mutex_lock(&mutex_A);

    // Decrement the counter for file A
    count_A--;

    // Unlock the mutex for file A
    pthread_mutex_unlock(&mutex_A);

    // Print the message that the thread releases file A
    printf("Thread h1 number %d (id.%lu) releases file A (%d %d)\n", num, id, count_A, count_B);

    // Signal the condition that file A is available
    pthread_cond_signal(&cond_A);

    // Return NULL
    return NULL;
}

// Define the function for H2 threads
void *h2(void *arg)
{
    // Get the thread number from the argument
    int num = *((int *)arg);

    // Get the thread id
    pthread_t id = pthread_self();

    // Lock the mutex for file B
    pthread_mutex_lock(&mutex_B);

    // Wait for the condition that no H3 thread is accessing file B
    while (count_B > 0)
    {
        pthread_cond_wait(&cond_B, &mutex_B);
    }

    // Increment the counter for file B
    count_B++;

    // Unlock the mutex for file B
    pthread_mutex_unlock(&mutex_B);

    // Print the message that the thread is accessing file B
    printf("Thread h2 number %d (id.%lu) accessing file B (%d %d)\n", num, id, count_A, count_B);

    // Simulate the use of file B
    sleep(rand() % 2);

    // Lock the mutex for file B
    pthread_mutex_lock(&mutex_B);

    // Decrement the counter for file B
    count_B--;

    // Unlock the mutex for file B
    pthread_mutex_unlock(&mutex_B);

    // Print the message that the thread releases file B
    printf("Thread h2 number %d (id.%lu) releases file B (%d %d)\n", num, id, count_A, count_B);

    // Signal the condition that file B is available
    pthread_cond_signal(&cond_B);

    // Return NULL
    return NULL;
}

// Define the function for H3 threads
void *h3(void *arg)
{
    // Get the thread number from the argument
    int num = *((int *)arg);

    // Get the thread id
    pthread_t id = pthread_self();

    // Lock the mutexes for both files A and B
    pthread_mutex_lock(&mutex_A);
    pthread_mutex_lock(&mutex_B);

    // Wait for the condition that no other thread is accessing either file A or B
    while (count_A > 0 || count_B > 0)
    {
        pthread_cond_wait(&cond_A, &mutex_A);
        pthread_cond_wait(&cond_B, &mutex_B);
    }

    // Increment the counters for both files A and B
    count_A++;
    count_B++;

    // Unlock the mutexes for both files A and B
    pthread_mutex_unlock(&mutex_A);
    pthread_mutex_unlock(&mutex_B);

    // Print the message that the thread is accessing both files A and B
    printf("Thread h3 number %d (id.%lu) accessing files A and B (%d %d)\n", num, id, count_A, count_B);

    // Simulate the use of both files A and B
    sleep(rand() % 2);

    // Lock the mutexes for both files A and B
    pthread_mutex_lock(&mutex_A);
    pthread_mutex_lock(&mutex_B);

    // Decrement the counters for both files A and B
    count_A--;
    count_B--;

    // Unlock the mutexes for both files A and B
    pthread_mutex_unlock(&mutex_A);
    pthread_mutex_unlock(&mutex_B);

    // Print the message that the thread releases both files A and B
    printf("Thread h3 number %d (id.%lu) releases files A and B (%d %d)\n", num, id, count_A, count_B);

    // Signal the conditions that both files A and B are available
    pthread_cond_signal(&cond_A);
    pthread_cond_signal(&cond_B);

    // Return NULL
    return NULL;
}

// Define the main function
int main()
{
    // Declare the arrays of thread ids for each type
    pthread_t h1_ids[NUM_H1];
    pthread_t h2_ids[NUM_H2];
    pthread_t h3_ids[NUM_H3];

    // Declare the arrays of thread numbers for each type
    int h1_nums[NUM_H1];
    int h2_nums[NUM_H2];
    int h3_nums[NUM_H3];

    // Initialize the random seed

    // Create the threads for each type
    for (int i = 0; i < NUM_H1; i++)
    {
        h1_nums[i] = i;
        pthread_create(&h1_ids[i], NULL, h1, &h1_nums[i]);
    }
    for (int i = 0; i < NUM_H2; i++)
    {
        h2_nums[i] = i;
        pthread_create(&h2_ids[i], NULL, h2, &h2_nums[i]);
    }
    for (int i = 0; i < NUM_H3; i++)
    {
        h3_nums[i] = i;
        pthread_create(&h3_ids[i], NULL, h3, &h3_nums[i]);
    }

    // Join the threads for each type
    for (int i = 0; i < NUM_H1; i++)
    {
        pthread_join(h1_ids[i], NULL);
    }
    for (int i = 0; i < NUM_H2; i++)
    {
        pthread_join(h2_ids[i], NULL);
    }
    for (int i = 0; i < NUM_H3; i++)
    {
        pthread_join(h3_ids[i], NULL);
    }

    // Return 0
    return 0;
}
