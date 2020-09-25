#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    int nsize;
    int myrank;
    int my_name_len;
    char my_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Get_processor_name(my_name, &my_name_len);

    printf("nsize = %d\n", nsize);
    printf("myrank = %d\n", myrank);
    printf("my_name_len = %d\n", my_name_len);
    printf("my_name = %s\n", my_name);

    MPI_Finalize();

    return 0;
}
