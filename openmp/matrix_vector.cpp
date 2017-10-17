#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
using namespace std;
#define size 100

void matrix_vector(int **matrix,int *vector,int m,int n){
    int result[n];
    int thread_count = 10;
    int i,j;
#pragma omp parallel for num_threads(thread_count) default(none) private(i, j) shared(matrix, vector, result, m, n)
    for(i=0; i<m; i++){
        result[i] = 0;
        for(j=0; j<n; j++)
            result[i] += matrix[i][j]*vector[j];
    }
}

int main(){
    int row=10000,column=10000;
	srand(time(NULL));
	int **a = new int*[row];
	for(int i=0; i < row; i++) {a[i] = new int[column];};
	for(int i=0; i < row; i++){
		for(int j=0; j < column; j++)
			a[i][j] = rand() % 1000;
	}

    int *b = new int[row];
    for(int i=0; i < row; i++){
			b[i] = rand() % 1000;
	}

    double start = omp_get_wtime();
    matrix_vector(a,b,row,column);
	printf("Time: \t %f \n", omp_get_wtime()-start);


    return 0;
}
