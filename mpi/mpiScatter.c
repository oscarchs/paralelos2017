#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
    int size, rank;

    MPI_Init(&argc, &argv); MPI_Comm_size(MPI_COMM_WORLD, &size);   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *globaldata=NULL;   int localdata;

    if (rank == 0) {
        globaldata = malloc(size * sizeof(int) );
        for (int i=0; i<size; i++)      globaldata[i] = 2*i+1;

        printf("Proceso Master tiene los datos %d : ", rank);
        // saber que procesos inician en MASTER
        for (int i=0; i<size; i++)      printf("%d ", globaldata[i]);
        printf("\n");
    }

    // metodo scatter para dividir la cantidad en MASTER
    MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Proceso %d tiene datos  %d\n", rank, localdata);
    localdata *= 2;
    printf("Proceso %d duplica los datos, ahora tiene %d\n", rank, localdata);
    // metodo gather para juntar la cantidad dividida en MASTER
    MPI_Gather(&localdata, 1, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Proceso Master %d tiene datos: ", rank);
        for (int i=0; i<size; i++)      printf("%d ", globaldata[i]);
        printf("\n");
    }
    if (rank == 0)      free(globaldata);
    MPI_Finalize();
    return 0;
}