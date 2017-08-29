#include <iostream>
#include <time.h>
using namespace std;


#define MAX 10000
double A[MAX][MAX], X[MAX], y[MAX];

void loops1(){
    for(int i=0;i < MAX; i++){
        for(int j=0;j < MAX; j++){
            y[i] += A[i][j]*X[j];
        }
    }
}

void loops2(){
    for(int j=0;j < MAX; j++){
        for(int i=0;i < MAX; i++){
            y[i] += A[i][j]*X[j];
        }
    }
}

int main()
{
    clock_t t1,t2;
    t1 = clock();
    loops1();
    t2 = clock();
    float diff1 = ((float)t2-(float)t1);
    cout << (float)diff1<<endl;

    t1=clock();
    loops2();
    t2=clock();
    float diff2 = ((float)t2-(float)t1);
    cout << diff2;
    return 0;
}
