#include <stdio.h>
#include <stdlib.h>
#include<time.h>

int main()
{
	FILE *fp=NULL;
	srand(time(NULL));//
	int i=0;
	char a[23]={0};
	char *p=NULL;
	
	fp=fopen("setting.file","wb+");
	
	for(i=0;i<20;i++)
	{
		a[i]=rand()%25+97;
	}
	a[20]=':';
	p=a;
	printf("The name is:%s\n",p);
	fputs(p,fp);
	fclose(fp);
	
	return 0;
}
