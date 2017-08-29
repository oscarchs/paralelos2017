#include <iostream>
#include <algorithm>
#include <stdlib.h>  /* srand, rand */
#include <time.h>
using namespace std;

int ** matrix_product(int **a,int **b, int a_rows,int a_cols,int b_rows,int b_cols){
	int **pr = new int*[a_rows];
	for(int i=0; i < a_rows; i++){
		pr[i] = new int[a_cols];
	}

    for(int i=0; i < a_rows; i++){
		for(int j=0; j < b_cols; j++){
			for(int k=0; k < b_rows; k++){
				pr[i][j] += a[i][k] * b[k][j];
			}
		}
	}
	return pr;
}

int ** matrix_product2(int **a,int **b,int n,int blockSize){
	int **pr = new int*[n];
	for(int i=0; i < n; i++){
		pr[i] = new int[n];
	}
    for (int ii = 0; ii<n; ii+=blockSize) {
        for (int jj = 0; jj<n; jj+=blockSize) {
            for (int kk = 0; kk<n; kk+=blockSize) {
                for (int i = ii; i< min(ii+blockSize,n); i++) {
                    for (int j = jj; j< min(jj+blockSize, n); j++) {
                        for (int k = kk; k< min(kk+blockSize, n); k++) {
                            pr[i][j] += a[i][k] * b[k][j];
                        }
                    }
                }
            }
        }
    }
    return pr;
}

void print_matrix(int **a,int a_rows,int a_cols){
	for(int i=0; i < a_rows; i++){
		for(int j=0; j < a_cols ; j++){
			cout << a[i][j]<<" | ";
		}
		cout << endl;
	}
}


int main(){

	int row=100,column=100;
	srand(time(NULL));
	int **a = new int*[row];
	for(int i=0; i < row; i++) {a[i] = new int[column];};
	for(int i=0; i < row; i++){
		for(int j=0; j < column; j++)
			a[i][j] = rand() % 1000;
	}

	int **b = new int*[row];
	for(int i=0; i < row; i++) {b[i] = new int[column];};
	for(int i=0; i < row; i++){
		for(int j=0; j < column; j++)
			b[i][j] = rand() % 1000;
	}

	clock_t t1,t2;
    t1 = clock();

	int ** m = matrix_product(a,b,row,row,column,column);
//	int ** m = matrix_product2(a,b,row,10);

    t2 = clock();
    float diff1 = ((float)t2-(float)t1);
    cout << (float)diff1/CLOCKS_PER_SEC<<endl;


	//print_matrix(b,row,column);

	return 0;
}
