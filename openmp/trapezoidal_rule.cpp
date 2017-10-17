//Trapezoidal Method for the evaluation of Definite Integrals
#include<iostream>
#include<cmath>
#include <omp.h>
using namespace std;
double f(double x){        //write the function whose definite integral is to be calcuated here
    return x*x;
}

double trapezoidal(double a, double b, int n) {
    double x;
    double integral = 0.0;
    double h = (b-a)/n;
    int i;
    int thread_count = n;
#  pragma omp parallel for default(none) schedule(static) shared(a, h, n) private(i, x) num_threads(thread_count)  reduction(+: integral)
    for (i = 1; i <= n-1; i++){
       x = a + i*h;
       integral += f(x);
    }
    integral += (f(a) + f(b))/2.0*h;
    cout << integral;
    return integral;
}

int main(){
    trapezoidal(10,1,10);
}
