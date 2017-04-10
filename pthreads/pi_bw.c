#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

const int MAX_THREADS = 1024;
long thread_count;      long long n;
int flag;               double sum;

void* Thread_sum(void* rank) {
   long my_rank = (long) rank;
   double factor, my_sum = 0.0;
   long long i;
   long long my_n = n/thread_count;
   long long my_first_i = my_n*my_rank;
   long long my_last_i = my_first_i + my_n;

   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;

   for (i = my_first_i; i < my_last_i; i++, factor = -factor) 
      my_sum += factor/(2*i+1);  
   
   while (flag != my_rank);
   sum += my_sum;  
   flag = (flag+1) % thread_count;

   return NULL;
}  

int main(int argc, char* argv[]) {
   long       thread;  
   pthread_t* thread_handles;
   double start, finish, elapsed;
   thread_count = strtol(argv[1], NULL, 10);  
   n = strtoll(argv[2], NULL, 10);
   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t)); 
   
   GET_TIME(start);
   sum = 0.0;
   flag = 0;
   for (thread = 0; thread < thread_count; thread++)  
      pthread_create(&thread_handles[thread], NULL,Thread_sum, (void*)thread);  

   for (thread = 0; thread < thread_count; thread++) 
      pthread_join(thread_handles[thread], NULL); 
   GET_TIME(finish);
   elapsed = finish - start;

   sum = 4.0*sum;
   printf("Con n = %lld terminos,\n", n);
   printf("   Calculo estimado de pi  = %.15f\n", sum);
   printf("   Tiempo transcurrido = %e segundos\n", elapsed);
   
   free(thread_handles);
   return 0;
} 