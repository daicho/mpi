#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

typedef long long int ll;
typedef long double ld;

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    unsigned int seed;
    ll i;
    ll n;
    ll cnt = 0;
    ll sum = 0;

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

    // 試行回数
    n = atoll(argv[1]);

    if (n <= 0) {
        printf("引数の値が不正です。\n");

        MPI_Finalize();
        return 0;
    }

    start_t = MPI_Wtime();

    // シード値決定
    srandom(time(NULL));
    for (i = 0; i < myrank; i++)
        random();

    seed = random();
    srandom(seed);

    printf("CPU %02d seed: %u\n", myrank, seed);

    // 処理開始
    for (i = 0; i < n / nsize; i++) {
        ld x = (random() / ((ld)RAND_MAX + 1) + myrank) / nsize;
        ld y = random() / ((ld)RAND_MAX + 1);

        if (x * x + y * y < 1)
            cnt++;
    }

    // 計算結果の統合
    MPI_Reduce(&cnt, &sum, 1, MPI_INTEGER8, MPI_SUM, 0, MPI_COMM_WORLD);

    printf("CPU %02d count: %lld\n", myrank, cnt);

    if (myrank == 0) {
        // 端数の処理
        for (i = 0; i < n % nsize; i++) {
            ld x = random() / ((ld)RAND_MAX + 1);
            ld y = random() / ((ld)RAND_MAX + 1);

            if (x * x + y * y < 1)
                sum++;
        }

        ld ans = (ld)4.0 * sum / n;

        finish_t = MPI_Wtime();

        printf("answer: %.20Lf\n", ans);
        printf("error : %.20Lf\n", ans - M_PI);
        printf("time  : %.4f s\n", finish_t - start_t);
    }

    MPI_Finalize();

    return 0;
}
