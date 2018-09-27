#include<iostream>
#include<cstdio>
#include<string>
#include<stdlib.h>
using namespace std;

string buf;
int main(void){

int color = 33;


 string cbuf = "\33[1;";
 cbuf += to_string(33);
 cbuf += "m";


// string k = to_string(33);



char temp1[10] = "abc\n";
string temp2 = "def";


string buf =  temp1 + temp2;

cout<<buf<<endl;

  string s = "abcdefg";
  string x = "xxxxx";
  s.insert(1,x);
  cout<<"insert"<<s<<endl;


  buf ="\33[1;35m hey \33[1;31m\n";
  cout<<buf<<endl;
  int a;
  cin>>a;
//  printf("%c[1;5m",27);
//  int a;
//  scanf("%d",&a);
//  printf("%c[1;30m",27);
  return 0;
}

