#include <mpi.h>
#include <stdio.h>
void sortEvenOdd(int a[],int n){
	int phase,i,temp;
	for(phase=0;phase<n;phase++){
		if(phase%2 == 0){
			for(i=1;i<n;i+=2){
				if(a[i-1]>a[i]){
                    temp = a[i];    a[i] = a[i-1];  a[i-1]=temp;
		}   }   }
		else{
			for(i=1;i<n-1;i+=2){
				if(a[i]>a[i+1]){
		        	temp=a[i];      a[i]=a[i+1];    a[i+1]=temp;
		}   }   }
	}
}


int main(){
	int vector[20]={20,12,0,54,56,67,5,2,3,72,9,1,4,7,8,100,121,1213,121,12};
	sortEvenOdd(vector,20);
    for(int i=0;i<20;i++){
        printf("%d", vector[i]);printf(" ");
	}
	return 0;
}