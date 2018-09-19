#include<iostream>
#include<cstdio>
#include<string>
#include<stdlib.h>
using namespace std;

string buf;
int main(void){

int color = 33;

 string buf ="Hello";
 string cbuf = "\33[1;";
 cbuf += to_string(33);

 cbuf += "m";
 cbuf += buf;


 string k = to_string(33);

 cout<<k<<endl;
 cout<<cbuf<<endl;

  buf ="\33[1;31mHey";
  cout<<buf<<endl;

//  printf("%c[1;5m",27);
//  int a;
//  scanf("%d",&a);
//  printf("%c[1;30m",27);
  return 0;
}

