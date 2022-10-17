#include<stdio.h>
#include<stdlib.h>
#include<base.h>
#include<window.h>

int main()
{
	char*p=0;
	window_t *w=0;
	desktop();
	border(0,0,24,79,WHITE,2);
	name(0,0,80,"T U R B O C",BLUE);
	clear(8,8,12,18,'?',WHITE);
	p=create_buffer(0,0,100,100);
	get_vmem (8,8,12,18,p);
	put_vmem (16,16,20,26,p);
	write_char(2,2,'T',BLUE);
	write_char(3,3,'U',BLUE);
	write_char(4,4,'R',BLUE);
	write_char(5,5,'B',BLUE);
	write_char(6,6,'O',BLUE);
	write_char(7,7,'C',WHITE);
	write_string(10,35,"PROGRAMMING",BLUE);
	w=window_create("TURBOC",5,35,20,60,BLUE|BGBLACK,1);
	window_open(w);
	window_xy(w,0,0);
	window_putchar(w,'C');
	window_xy(w,10,0);
	window_gets(w,200);
	/*window_close(w);
	window_destroy(w);*/
return 0;
}

