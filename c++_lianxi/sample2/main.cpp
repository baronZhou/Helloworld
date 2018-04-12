#include <iostream>
#include <string>

using namespace std;

void swap(int & x, int & y)
{
	int temp;
	temp=x;
	x=y;
	y=temp;
}

void calculate_count(string str)
{
	char a[27]="abcdefghijklmnopqrstuvwxyz";
	char b[27]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int upper[26]={0};
	int lower[26]={0};
	char *p=(char *)str.c_str();
	int i=0;
	
	while(*p != 0)
	{
		if(*p>='a' && *p<='z')
			lower[*p-'a']++;
		else if(*p>='A' && *p<='Z')
			upper[*p-'A']++;
		p++;
	}
	
	for(i=0;i<26;i++)
		if(upper[i] != 0)
			cout<<b[i]<<":"<<upper[i]<<endl;
	for(i=0;i<26;i++)
		if(lower[i] != 0)
			cout<<a[i]<<":"<<lower[i]<<endl;
	
}

void string_operation()
{
	string strn="\n";
	string str1="Two roads diverged in a yellow wood,";
	string str2="And sorry I could not travel both";
	string str3="And be one traveler, long I stood";
	string str4="And looked down one as far as I could";
	string str5="To where it bent in the undergrowth;";

	string stro=str1+strn+str2+strn+str3+strn+str4+strn+str5;

	cout<<"***********string test ***************"<<endl;
	cout<<"str="<<stro<<endl;
	cout<<"---------------"<<endl;
	calculate_count(stro);
	cout<<"**************************************"<<endl;
}

int main(int arg,char **argv)
{
	int x=5,y=10;
	
	swap(x,y);
	 string_operation();

	cout<<"x="<<x<<",y="<<y<<endl;
	cout<<"end"<<endl;
}
