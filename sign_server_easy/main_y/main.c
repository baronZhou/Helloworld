#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<time.h>

#define NAME_LENTH 10
int generate();

int main(int arv,char *args[])
{
#if 0
	generate();
#else
	
	FILE *fp=NULL;
	FILE *fp2=NULL;
	char buf[NAME_LENTH+5]={0};
	char buf2[NAME_LENTH+5]={0};
	char *rc=NULL;
	
	fp=fopen("limits.file","r+");
	
	//printf("arv=%d,args=%s\n",arv,args[1]);
	while(1)
	{
		rc=fgets(buf2,NAME_LENTH+5,fp);
		
		strcpy(buf,buf2);
		buf[NAME_LENTH]=0;
		
		printf("buf=%s,args=%s\n",buf,args[1]);
		if(strcmp(buf,args[1]) == 0)
		{
			printf("1111:%c\n",buf[NAME_LENTH+1]);
			
			if(buf[NAME_LENTH+1]>='1')
			{
				buf2[NAME_LENTH+1]=buf[NAME_LENTH+1]-1;
				fseek(fp, -(NAME_LENTH+3), SEEK_CUR);
				fputs(buf2,fp);
				fp2=fopen("result.file","wb+");fputs("1:",fp2);fclose(fp2);
			}
			else
			{
				fp2=fopen("result.file","wb+");fputs("0:",fp2);fclose(fp2);
			}
			
			
			break;
		}
		
		
		if(rc == NULL)
		{
			printf("2222\n");
			printf("buf=%s,args=%s\n",buf,args[1]);
			//fp2=fopen("result.file","wb+");fputs("0:",fp2);fclose(fp2);
			break;
		}
	
	}
	
	fclose(fp);
#endif
}

int generate()
{
	FILE *fp=NULL;
	srand(time(NULL));//
	int i=0,j=0;
	char a[NAME_LENTH+5]={0};
	char *p=NULL;
	
	fp=fopen("limits.file","wb+");
	
for(j=0;j<100;j++)
{
	for(i=0;i<NAME_LENTH;i++)
	{
		a[i]=rand()%25+97;
	}
	a[NAME_LENTH]=':';
	a[NAME_LENTH+1]='5';
	a[NAME_LENTH+2]='\n';
	a[NAME_LENTH+3]=0;
	p=a;
	printf("The name iis:%s\n",p);
	fputs(p,fp);
}
	
	fclose(fp);
	
	return 0;
}
