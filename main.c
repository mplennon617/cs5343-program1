#include <stdio.h>
#include <pthread.h>

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
 * TODO: Update runner description
 *
 * Called when each thread is created.
 */
void* runner(void* param) {


}

/*
 * TODO: Update read_file description
 *
 */
void read_file(FILE* fp) {

    int num_threads = 0;
    int num_rounds = 0;

    // Read the first line of the file. (Get # of threads to create and # of rounds to score)
    fscanf(fp, "%d", &num_threads);
    fscanf(fp, "%d", &num_rounds);

    // For each thread...
    for (int i = 0; i < num_threads; i++) {

        // Create a new thread, passing in 'i' as the paramter. (Starts from 0 to n-1)
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);

        //pthread_create(&tid, &attr, runner, &i);

        // For each round...
        for (int j = 0; j < num_rounds; j++) {
            printf("Main process start round %d\n",j);
        }
    }
}

/*
 * f1 - A function used to generate pseudorandom numbers.
 *
 * @param argc - Number of command-line arguments.
 * @param argv -
 *          argv[1] - The name of the file to read from (A .txt file in the same directory).
 * @param c - A user-defined constant.
 * @param m - A user-defined constant.
 * @return - An integer denoting the next pseudorandom number.
 */
int main(int argc, char** argv) {

    if (argc <= 1) {
        printf("ERROR HANDLING HERE\n");
    } else {
        FILE *fp;

        int i;
        printf("%d\n",argc);
        for(i=1;i<argc;i++)
        {
            printf("%s",argv[i]);
        }

        // TODO: ADD ERROR HANDLING IF THE FILE NAME DOES NOT EXIST.
        fp = fopen(argv[1],"r");

        read_file(fp);
        fclose(fp);
    }

    printf("Hello, World!\n");
    return 0;
}
