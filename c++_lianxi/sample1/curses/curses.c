#include <curses.h>

//#define MY_PRINTK(fmt, ...)           printf("[Curses:]"fmt, ##__VA_ARGS__)
#define MY_PRINTK(fmt, ...) { \
		char str[100]; \
		sprintf(str,"[Curses:]"fmt, ##__VA_ARGS__); \
		FILE *fp=fopen("log.txt","a+"); \
		fputs(str,fp); \
		fclose(fp); \
		}


static void initial()
{
    initscr();                    //开启curses模式
    cbreak();                     //开启cbreak模式，除 DELETE 或 CTRL 等仍被视为特殊控制字元外一切输入的字元将立刻被一一读取
    nonl();                       //用来决定当输入资料时，按下 RETURN 键是否被对应为 NEWLINE 字元
    noecho();                     //echo() and noecho(): 此函式用来控制从键盘输入字元时是否将字元显示在终端机上
    intrflush(stdscr,false);
    keypad(stdscr,true);          //当开启 keypad 後, 可以使用键盘上的一些特殊字元, 如上下左右>等方向键
   // refresh();                    //将做清除萤幕的工作	
}


int operation()
{
	
	 getch();
}

int static_car(int x,int y)
{
	
	attron(COLOR_PAIR(1)); /*开启字符输出颜色*/
	move(x, y);  waddstr(stdscr, "_____"); 
	move(x+1, y);waddstr(stdscr, "|    \\"); 
	move(x+2, y);waddstr(stdscr, "|   > \\");
	move(x+3, y);waddstr(stdscr, "-o--o--");
	attroff(COLOR_PAIR(1)); /*关闭颜色显示*/
	
}

int init_window()
{
	MY_PRINTK(" enter %s\n",__func__);
	initial();
	
	start_color();
	init_pair(1, COLOR_RED, COLOR_GREEN); /*建立一个颜色对*/
	
	box(stdscr, ACS_VLINE, ACS_HLINE); /*draw a box*/
    move(LINES/2, COLS/2); /*move the cursor to the center*/
    waddstr(stdscr, "Hello, world!");
	static_car(20,20);
    refresh();
   // getch();
   // endwin();
	
	return 0;
}
int close_window()
{
	MY_PRINTK(" enter %s\n",__func__);
    endwin();
	return 0;
}