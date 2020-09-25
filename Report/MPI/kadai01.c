#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

typedef long long int ll;

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    ll i;
    ll num;
    ll sum = 0;
    ll ans, max, min;

    double start_t;
    double finish_t;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    if (argc <= 1) {
        printf("引数が与えられていません。\n");

        MPI_Finalize();
        return 0;
    }

    num = atoll(argv[1]);

    if (num <= 0) {
        printf("引数の値が小さすぎます。\n");

        MPI_Finalize();
        return 0;
    }

    if (num > 4000000000ll) {
        printf("引数の値が大きすぎます。\n");

        MPI_Finalize();
        return 0;
    }

    start_t = MPI_Wtime();

    for (i = 1 + myrank; i <= num; i += nsize) {
        sum += i;
    }

    MPI_Reduce(&sum, &ans, 1, MPI_INTEGER8, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum, &max, 1, MPI_INTEGER8, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&sum, &min, 1, MPI_INTEGER8, MPI_MIN, 0, MPI_COMM_WORLD);

    finish_t = MPI_Wtime();

    if (myrank == 0) {
        printf("answer: %lld\n", ans);
        printf("max: %lld\n", max);
        printf("min: %lld\n", min);
        printf("time: %f\n", finish_t - start_t);
    }

    MPI_Finalize();
    
    return 0;
}
