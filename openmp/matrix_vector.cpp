#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
using namespace std;
#define size 100

void matrix_vector(int **matrix,int *vector,int m,int n){
    int result[n];
    int thread_count = 4;
    int i,j;
#pragma omp parallel for num_threads(thread_count) default(none) private(i, j) shared(matrix, vector, result, m, n)
    for(i=0; i<m; i++){
        result[i] = 0;
        for(j=0; j<n; j++)
            result[i] += matrix[i][j]*vector[j];
    }
}

int main(){
    int m=100,n=1000000;
	srand(time(NULL));

	int **matrix= new int*[m];
    for(int i=0; i<m; i++){
        matrix[i] = new int[n];
        for(int j=0; j<n; j++){
            matrix[i][j] = rand() % 999;
        }
    }

    int *vector = new int[n];
    for(int i=0; i < n; i++){
			vector[i] = rand() % 999;
	}

    double start = omp_get_wtime();
    matrix_vector(matrix,vector,m,n);
	printf("Time: \t %f \n", omp_get_wtime()-start);


    return 0;
}
