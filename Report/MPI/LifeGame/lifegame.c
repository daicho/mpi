#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>

#define H 10
#define W 2

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    int i, j;
    int x, y;

    srand(time(NULL) + myrank * 100);

    // マップの状態
    int p_cells[H + 2][H];
    int cells[W][H] = {{0}};

    // ランダムに配置
    for (y = 0; y < H; y++) {
        for (x = 0; x < W; x++) {
            cells[x][y] = rand() % 2;
        }
    }

    int gen = 0;

    while (1) {
        // 前回の状態を保存
        for (y = 0; y < H; y++) {
            for (x = 0; x < W; x++) {
                p_cells[x + 1][y] = cells[x][y];
            }
        }

        // のりしろ部分の通信
        MPI_Status status;

        int l_rank = (myrank - 1 + nsize) % nsize;
        int r_rank = (myrank + 1 + nsize) % nsize;

        MPI_Send(cells[0], H, MPI_INTEGER, l_rank, 100 + myrank, MPI_COMM_WORLD);
        MPI_Send(cells[W - 1], H, MPI_INTEGER, r_rank, 200 + myrank, MPI_COMM_WORLD);

        MPI_Recv(p_cells[W + 1], H, MPI_INTEGER, r_rank, 100 + r_rank, MPI_COMM_WORLD, &status);
        MPI_Recv(p_cells[0], H, MPI_INTEGER, l_rank, 200 + l_rank, MPI_COMM_WORLD, &status);

        // 更新
        for (y = 0; y < H; y++) {
            for (x = 0; x < W; x++) {
                // 周囲の生存セルをカウント
                int cnt = 0;

                for (i = -1; i <= 1; i++) {
                    for (j = -1; j <= 1; j++) {
                        if (i == 0 && j == 0) continue;
                        if (p_cells[x + j + 1][(y + i + H) % H]) cnt++;
                    }
                }

                if (cnt == 3) {
                    // 誕生
                    cells[x][y] = 1;
                } else if (cnt <= 1 || cnt >= 4) {
                    // 死滅
                    cells[x][y] = 0;
                }
            }
        }

        // 集約して表示
        if (myrank == 0) {
            int all_cells[W * nsize][H];

            for (y = 0; y < H; y++) {
                for (x = 0; x < W; x++) {
                    all_cells[x][y] = cells[x][y];
                }
            }

            for (i = 1; i < nsize; i++) {
                for (x = 0; x < W; x++) {
                    MPI_Recv(all_cells[i * W + x], H, MPI_INTEGER, i, 300 + i, MPI_COMM_WORLD, &status);
                }
            }

            printf("\033[;H\033[2J");
            printf("Generation: %d\n", gen);

            for (y = 0; y < H; y++) {
                for (x = 0; x < W * nsize; x++) {
                    if (all_cells[x][y])
                        printf("@");
                    else
                        printf(".");
                }

                printf("\n");
            }
        } else {
            for (x = 0; x < W; x++)
                MPI_Send(cells[x], H, MPI_INTEGER, 0, 300 + myrank, MPI_COMM_WORLD);

            usleep(100000);
        }

        gen++;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
