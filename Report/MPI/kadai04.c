#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

#define N 6

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    unsigned int seed;
    int i, j;

    double start_t;
    double finish_t;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    start_t = MPI_Wtime();

    // シード値決定
    srandom(time(NULL));
    for (i = 0; i < myrank; i++)
        random();

    seed = random();
    srandom(seed);

    if (myrank == 0) {
        int a[N] = {3, 1, 4, 1, 5, 9};

        // aを送信
        for (i = 1; i < nsize; i++)
            MPI_Send(a, N, MPI_INTEGER, i, 100 + i, MPI_COMM_WORLD);

        for (i = 1; i < nsize; i++) {
            int R;
            int b[N + 1];
            MPI_Status status;

            // Rとbを受信
            MPI_Recv(&R, 1, MPI_INTEGER, i, 200 + i, MPI_COMM_WORLD, &status);
            MPI_Recv(b, N + 1, MPI_INTEGER, i, 300 + i, MPI_COMM_WORLD, &status);

            // 表示
            printf("CPU %02d\n", i);
            printf("R: %d\n", R);
            printf("b:");

            for (j = 0; j < N + 1; j++)
                printf(" %d", b[j]);
            printf("\n");
        }

        // 処理時間を表示
        finish_t = MPI_Wtime();
        printf("time: %.4f s\n", finish_t - start_t);
    } else {
        int a[N];
        int R;
        int b[N + 1];
        MPI_Status status;

        // aを受信
        MPI_Recv(a, N, MPI_INTEGER, 0, 100 + myrank, MPI_COMM_WORLD, &status);

        // aにRを追加したものをbに格納
        for (i = 0; i < N; i++)
            b[i] = a[i];

        R = random() % 100;
        b[N] = R;

        // bを送信
        MPI_Send(&R, 1, MPI_INTEGER, 0, 200 + myrank, MPI_COMM_WORLD);
        MPI_Send(b, N + 1, MPI_INTEGER, 0, 300 + myrank, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
