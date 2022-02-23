// Michael Lennon
// CS 5343 - Program 1
// February 22, 2022

/*
 * Description: A program which creates threads and keeps track of the scores of all threads. Threads
 * score points by generating a random number via a generator function f1() that is higher (or lower) than
 * all other threads. The program will display the score of all threads -- along with the winner(s) -- when
 * all rounds have concluded.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// -----------------------------------------------------------------------------------------------------
//                                   Global Variables and Structs
// -----------------------------------------------------------------------------------------------------

// An 'option code' to determine what the thread function will do when run. Set via the main process.
// 1 - Generate f1.
// 2 - Quit.
char* message;

// Keep track of the current random number generated by each thread, as well as each thread's score.
int* thread_curr_rand_numbers;
int* thread_scores;
int* thread_numbers_generated;

// We keep track of how many numbers have been generated in total.
// Used to keep track of when all threads have submitted their numbers for a round.
int numbers_generated = 0;

/*
 * Struct to keep track of necessary random parameters. Passed into each thread
 * and used as parameters for f1().
 *
 * @param thread_num - used to keep track of which thread we're running.
 * @param x - The Previous randomly generated number
 * @param a - A user-defined constant.
 * @param c - A user-defined constant.
 * @param m - A user-defined constant.
 */
struct RandParams {
    int thread_num;
    int a;
    int c;
    int m;
    int x0;
};

// -----------------------------------------------------------------------------------------------------
//                                      f1 and other Helper Functions
// -----------------------------------------------------------------------------------------------------

/*
 * f1 - A function used to generate pseudorandom numbers.
 *
 * @param x - The Previous randomly generated number
 * @param a - A user-defined constant.
 * @param c - A user-defined constant.
 * @param m - A user-defined constant.
 * @return - An integer denoting the next pseudorandom number.
 */
int  f1(int x, int a, int c, int m) {
    long int x1 = x * a + c;
    return (int)(x1 % m);
}

/*
 * A helper method to sum all the elements in an integer array.
 *
 * @param size - size of the array.
 * @param arr - the array.
 * @return - the sum of all elements in the array
 */
int sum(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// -----------------------------------------------------------------------------------------------------
//                                          Thread Function
// -----------------------------------------------------------------------------------------------------

/*
 * The thread function. Called upon thread creation. Option codes dictate which action each thread performs.
 * op1 - call f1() with rand_param values (passed in as param).
 * op2 -
 *
 * @param param - a RandParam struct which contains thread_num (keeping track of scoring and number generation) and
 *                a, c, m, x0 (f1 parameters).
 * @return x1 - the number generated by f1().
 */
void* runner(void* param) {

    struct RandParams* rand_params = (struct RandParams*)param;
    int curr_x = rand_params->x0;

    // As long as the main process doesn't quit the thread...
    while (strcmp(message,"quit") != 0) {
        // Don't begin generating numbers until the main message tells the thread to stop waiting.
        // TODO: Ask Dr Lin if there is a nicer "pthread-esque" alternative to what was done below.
        while (strcmp(message,"wait") == 0) {}

        // Generate the next number, then store it in thread_curr_rand_numbers.
        curr_x = f1(curr_x, rand_params->a, rand_params->c, rand_params->m);
        thread_curr_rand_numbers[rand_params->thread_num] = curr_x;
        printf("Thread %d call f1() returns %d\n",rand_params->thread_num,curr_x); // Required : Printing f1() value
        sleep(2);
        thread_numbers_generated[rand_params->thread_num]++;
    }

    // When message has been set to quit, we can exit.
    pthread_exit(0);
}

// -----------------------------------------------------------------------------------------------------
//                                          Scoring functions
// -----------------------------------------------------------------------------------------------------

void determine_winners(int num_threads) {
    int max = thread_scores[0];

    // Determine the highest score.
    for (int i = 0; i < num_threads; i++) {
        printf("Score for thread %d: %d\n",i,thread_scores[i]);
        if (thread_scores[i] > max) {
            max = thread_scores[i];
        }
    }

    // Required: Print all threads that scored the highest, in ascending order.
    printf("Overall winner: ");
    for (int i = 0; i < num_threads; i++) {
        if (thread_scores[i] == max) {
            printf("%d ", i);
        }
    }

    printf("\n");
}

/*
 * Function used to score a round after it is complete.
 *
 * @param num_threads -- number of thread_scores to check when scoring the round.
 */
void score_round(int num_threads, int round_num) {

    int min = thread_curr_rand_numbers[0];
    int max = thread_curr_rand_numbers[0];
    int smallest_thread_idx = 0;
    int largest_thread_idx = 0;
    // Keep track of the thread_num with the smallest and largest number.
    for (int i = 1; i < num_threads; i++) {
        // Keeping track of smallest number.
        if (thread_curr_rand_numbers[i] < min) {
            smallest_thread_idx = i;
            min = thread_curr_rand_numbers[smallest_thread_idx];
        }
        // If the current random number is equal to min, there's a chance it may be a tie.
        // Set index out of bounds until we find a new smaller value (or not).
        else if (thread_curr_rand_numbers[i] == min) {
            smallest_thread_idx = -1;
        }

        // Keeping track of largest number.
        if (thread_curr_rand_numbers[i] > max) {
            largest_thread_idx = i;
            max = thread_curr_rand_numbers[largest_thread_idx];
        }
        // If the current random number is equal to max, there's a chance it may be a tie.
        // Set index out of bounds until we find a new larger value (or not).
        else if (thread_curr_rand_numbers[i] == max) {
            largest_thread_idx = -1;
        }
    }

    // Award 1 point to the smallest and largest random number.
    // Only award a point if there is no tie.
    if (smallest_thread_idx >= 0) {
        thread_scores[smallest_thread_idx]++;
    }

    if (largest_thread_idx >= 0) {
        thread_scores[largest_thread_idx]++;
    }

    // Required : Printing threads that earned a point
    printf("Round %d finished. Winners are ", round_num);

    // Required for printing -- print out the smallest THREAD ID (not thread with smallest number) first.
    if (smallest_thread_idx >= 0 && largest_thread_idx >= 0) {
        if (largest_thread_idx < smallest_thread_idx) {
            int temp = largest_thread_idx;
            largest_thread_idx = smallest_thread_idx;
            smallest_thread_idx = temp;
        }
    }
    if (smallest_thread_idx >= 0) {
        printf("%d", smallest_thread_idx);
    }
    printf(" ");
    if (largest_thread_idx >= 0) {
        printf("%d", largest_thread_idx);
    }
    printf("\n");
}

// -----------------------------------------------------------------------------------------------------
//                              File Reading; Thread Creation and Management
// -----------------------------------------------------------------------------------------------------

/*
 * Reads in the file and creates and runs threads as it reads in information.
 * It then runs rounds, commences scoring (per round), then determines the winning thread.
 *
 * @param fp - Pointer to the file to read from.
 */
void create_and_run_threads(FILE* fp) {

    int num_threads = 0;
    int num_rounds = 0;

    // Read the first line of the file. (Get # of threads to create and # of rounds to score)
    fscanf(fp, "%d", &num_threads);
    fscanf(fp, "%d", &num_rounds);

    // Dynamically allocate global arrays.
    thread_curr_rand_numbers = (int*)malloc(sizeof(int) * num_threads);
    thread_scores = (int*)malloc(sizeof(int) * num_threads);
    memset(thread_scores, 0, sizeof(int) * num_threads);
    thread_numbers_generated = (int*)malloc(sizeof(int) * num_threads);
    memset(thread_numbers_generated, 0, sizeof(int) * num_threads);

    message = "wait";

    // Store tids and rand parameters for each thread.
    // tids - used for pthread_join after rounds are complete.
    // rand_params - passes thread_num,a,c,m,x0 into thread runner function.
    pthread_t tids[num_threads];
    struct RandParams rand_params[num_threads];

    // Create each thread.
    for (int i = 0; i < num_threads; i++) {

        int thread_num = i;

        // Read in initial values for the thread, and store in a RandParams struct.
        int a, c, m, x0;
        fscanf(fp, "%d", &a);
        fscanf(fp, "%d", &c);
        fscanf(fp, "%d", &m);
        fscanf(fp, "%d", &x0);

        rand_params[i].thread_num = thread_num;
        rand_params[i].a = a;
        rand_params[i].c = c;
        rand_params[i].m = m;
        rand_params[i].x0 = x0;

        // Create a new thread, passing in the thread's rand_params as the parameter.
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        pthread_create(&tid, &attr, runner, &rand_params[i]);

        // Store the tid in the tids array for pthread_join (called after rounds are scored).
        tids[i] = tid;
    }
    // For each round...
    for (int rnd = 0; rnd < num_rounds; rnd++) {
        printf("Main process start round %d\n",rnd+1);  // Required : Printing round number

        // Let the threads generate their numbers. Once they're generated, make them wait to generate a new one.
        message = "generate";
        sleep(1);
        message = "wait";
        // Dummy while loop. Waits as long as threads are in the process of submitting their numbers.
        // TODO: Ask Dr Lin if there is a nicer "pthread-esque" alternative to what was done below.
        while(sum(thread_numbers_generated,num_threads) % num_threads != 0) {}
        // Score the round when ready.
        score_round(num_threads, rnd+1);

        // Avoid the last sleep if we're on the last round, so we don't generate any excess numbers.
        if (rnd == num_rounds - 1) {
            message = "quit";
        }
        sleep(1);
    }

    // Set message to quit, then wait for all threads to end.
    message = "quit";
    for (int i = 0; i < num_threads; i++) {
        pthread_join(tids[i],NULL);
    }

    // Printing final statistics.
    determine_winners(num_threads);
}

// -----------------------------------------------------------------------------------------------------
//                                           Main function.
// -----------------------------------------------------------------------------------------------------

/*
 * Main function -- reads the file and scores the round.
 *
 * @param argc - Number of command-line arguments.
 * @param argv -
 */
int main(int argc, char** argv) {

    if (argc > 1) {
        FILE *fp;

        fp = fopen(argv[1],"r");

        if (fp != NULL) {
            printf("%s\n", argv[1]); // REQUIRED : Printing the name of the file.
            create_and_run_threads(fp);
        }

        fclose(fp);
    }

    return 0;
}
