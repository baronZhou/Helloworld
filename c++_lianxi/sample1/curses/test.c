#include <stdio.h>

#include "my_curses.h"

#define MY_PRINTK(fmt, ...) { \
		char str[100]; \
		sprintf(str,"[Test:]"fmt, ##__VA_ARGS__); \
		FILE *fp=fopen("log.txt","a+"); \
		fputs(str,fp); \
		fclose(fp); \
		}


int main()
{
	MY_PRINTK("********* main start ***********\n");
	
	init_window();
	//printf("1111\n");
	operation();
	
	close_window();

	return 0;
}
