#include<iostream> 
#include<stdlib.h> 
using namespace std;
class Sample 
{ 
    int x,y; 
public: 
    Sample() {x=y=0;} 
    Sample(int a,int b) {x=a;y=b;} 
   ~Sample() 
   { 
      if(x==y) 
          cout<<"x=y"<<endl; 
      else 
          cout<<"x!=y"<<endl; 
    } 
    void disp() 
    { 
        cout<<"x="<<x<<",y="<<y<<endl; 
    } 
}; 

int main() 
{ 
    Sample s1,s2(2,3); 
    s1.disp(); 
    s2.disp(); 
return 0;
}
