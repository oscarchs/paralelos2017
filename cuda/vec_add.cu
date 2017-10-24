#include <stdio.h>
#include <stdlib.h>
#include <math.h>

__global__ void Vec_add(float x[], float y[], float z[], int n) {
   int i = blockDim.x * blockIdx.x + threadIdx.x;
   if (i < n){
     z[i] = x[i] + y[i];
   }
 }

int main(int argc, char* argv[]) {
   int n, i;
   float *h_x, *h_y, *h_z;
   float *d_x, *d_y, *d_z;
   int threads_per_block;
   int block_count;
   size_t size;
   if (argc != 2) {
      fprintf(stderr, "usage: %s <vector order>\n", argv[0]);
      exit(0);
   }
   n = strtol(argv[1], NULL, 10);
   size = n*sizeof(float);

   h_x = (float*) malloc(size);
   h_y = (float*) malloc(size);
   h_z = (float*) malloc(size);

   for (i = 0; i < n; i++) {
      h_x[i] = i+1;
      h_y[i] = n-i;
   }

   cudaMalloc(&d_x, size);
   cudaMalloc(&d_y, size);
   cudaMalloc(&d_z, size);
   cudaMemcpy(d_x, h_x, size, cudaMemcpyHostToDevice);
   cudaMemcpy(d_y, h_y, size, cudaMemcpyHostToDevice);

   threads_per_block = 256;
   block_count = (n + threads_per_block - 1)/threads_per_block;
   Vec_add<<<block_count, threads_per_block>>>(d_x, d_y, d_z, n);
   cudaThreadSynchronize();
   cudaMemcpy(h_z, d_z, size, cudaMemcpyDeviceToHost);

   printf("The sum is: \n");
   for (i = 0; i < n; i++)
      printf("%.1f ", h_z[i]);
   printf("\n");

   cudaFree(d_x);
   cudaFree(d_y);
   cudaFree(d_z);
   free(h_x);
   free(h_y);
   free(h_z);
   return 0;
}
