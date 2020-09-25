#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mulproc.h"

// 精度(桁数)
#define PREC 1000

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    nsize = 1;
    myrank = 0;

    int i, j;
    int n;
    double start_t;
    double finish_t;
    struct NUMBER tmp;

    int a[] = {24, 8, 4};
    int b[] = {8, 57, 239};

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    if (argc <= 1) {
        printf("引数が与えられていません。\n");

        MPI_Finalize();
        return 0;
    }

    n = atoi(argv[1]);

    if (n <= 0) {
        printf("引数の値が不正です。\n");

        MPI_Finalize();
        return 0;
    }

    // 計測開始
    start_t = MPI_Wtime();

    // 精度
    struct NUMBER prec;
    setInt(&prec, 1);

    for (i = 0; i < PREC; i++) {
        mulByBase(&prec, &tmp);
        copyNumber(&tmp, &prec);
    }

    // 合計
    struct NUMBER sum;
    clearByZero(&sum);

    for (i = 0; i < 3; i++) {
        struct NUMBER base;
        struct NUMBER div;
        struct NUMBER upd;
        struct NUMBER bunshi;

        setInt(&base, b[i] * b[i]);
        setInt(&div, b[i]);
        setInt(&upd, 1);

        for (j = 0; j < myrank; j++) {
            multiple(&div, &base, &tmp);
            copyNumber(&tmp, &div);
        }

        for (j = 0; j < nsize; j++) {
            multiple(&upd, &base, &tmp);
            copyNumber(&tmp, &upd);
        }

        setInt(&bunshi, a[i]);
        multiple(&bunshi, &prec, &tmp);
        copyNumber(&tmp, &bunshi);

        for (j = myrank; j < n; j += nsize) {
            struct NUMBER bunbo;
            struct NUMBER calc;

            printf("CPU %d (i, j) = (%d, %d)\n", myrank, i, j);

            // 分母の計算
            setInt(&bunbo, 2 * j + 1);
            multiple(&bunbo, &div, &tmp);
            copyNumber(&tmp, &bunbo);

            // 分数の計算
            divide(&bunshi, &bunbo, &calc, &tmp);

            // 加減算
            if (j % 2 == 0)
                add(&sum, &calc, &tmp);
            else
                sub(&sum, &calc, &tmp);

            copyNumber(&tmp, &sum);

            // 割る数を更新
            multiple(&div, &upd, &tmp);
            copyNumber(&tmp, &div);
        }
    }

    if (myrank == 0) {
        MPI_Status status;

        int recv_n[KETA];
        int recv_sign;

        struct NUMBER ans;
        struct NUMBER recv;

        copyNumber(&sum, &ans);

        for (i = 1; i < nsize; i++) {
            MPI_Recv(recv_n, KETA, MPI_INTEGER, i, 100 + i, MPI_COMM_WORLD, &status);
            MPI_Recv(&recv_sign, 1, MPI_INTEGER, i, 200 + i, MPI_COMM_WORLD, &status);

            for (j = 0; j < KETA; j++)
                recv.n[j] = recv_n[j];
            recv.sign = recv_sign;

            add(&ans, &recv, &tmp);
            copyNumber(&tmp, &ans);
        }

        finish_t = MPI_Wtime();

        printf("time: %.4f s\n", finish_t - start_t);
        dispNumber(&ans);
    } else {
        MPI_Send(sum.n, KETA, MPI_INTEGER, 0, 100 + myrank, MPI_COMM_WORLD);
        MPI_Send(&sum.sign, 1, MPI_INTEGER, 0, 200 + myrank, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    return 0;
}
