#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void gen_matrices(int a){
  ofstream outfile ("in_a.txt");
  ofstream outfile1 ("in_b.txt");
  int total = a*a;
  for(int i=0; i < total; i++){
    if( i % a ==0 && i!=0){
      outfile<<'\n';
      outfile1<<'\n';
    }
    else{
      string number = to_string(i);
      outfile << number;
      outfile << ' ';
      outfile1 << number;
      outfile1<<' ';
    }
  }
}



int main(int argc, char* argv[]){
  gen_matrices( stoi(argv[1]) );
  return 0;
}
