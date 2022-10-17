#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<dos.h>
#include<string.h>
#include<base.h>

/*Системные переменные: v,VMEM
Прикладные переменные: ch,attrib*/
int write_char(int x,int y,char ch,char attrib)
{
	char *v=VMEM;
	v+=x*160+y*2;
	*v=ch;
	*v++;
	*v=attrib;
	return;
}

/*Системные переменные: i,end
Прикладные переменные: string,attrib*/
int write_string(int x,int y,char* string,char attrib)
{
	int i;
	int end=strlen(string);
	for(i=0;i<end;i++) {
		write_char(x,y+i,string[i],attrib);
	}
	return;
}

/*Системные переменные: x,y,startx,starty,endx,endy
Прикладные переменные: ch,attrib
Уведомление об ошибке когда конечные координаты будут меньше начальных*/
int clear(int startx,int starty,int endx,int endy,char ch,char attrib)
{
	int x,y;
	if(endx<=startx || endy<=starty) {fprintf(stderr,"Error: Invalid coordinates"); exit(1);}
	for(x=startx;x<=endx;x++) {
		for(y=starty;y<=endy;y++) {
			write_char(x,y,ch,attrib);
		}
	}
	return 0;
}

/*Системные переменные: v,startx,starty,endx,endy
Уведомление об ошибке когда конечные координаты будут меньше начальных*/
char* create_buffer(int startx,int starty,int endx,int endy)
{
	char* v;
	if(endx<=startx || endy<=starty) {fprintf(stderr,"Error: Invalid coordinates"); exit(1);}
	v=malloc((endx-startx+1)*(endy-starty+1)*2*sizeof(char));
	if (!v){
		exit (1);
	}
	return v;
}

/*Системные переменные: x,y,v,buffer,startx,starty,endx,endy,VMEM
Уведомление об ошибке когда конечные координаты будут меньше начальных или когда буфер
будет равен нулю*/
int get_vmem(int startx,int starty,int endx,int endy,char*buffer)
{
	int x,y;
	char  *v;
	if(buffer==NULL) {fprintf(stderr,"Error: Invalid buffer size"); exit(1);}
	if(endx<=startx || endy<=starty) {fprintf(stderr,"Error: Invalid coordinates"); exit(1);}
	v=create_buffer(startx,starty,endx,endy);
		for(x=startx;x<=endx;x++) {
			for(y=starty;y<=endy;y++) {
				v=VMEM;
				v+=x*160+y*2;
				*buffer=*v;
				*v++;
				*buffer++;
				*buffer=*v;
				*buffer++;
			}
		}
	return 0;
}

/*Системные переменные: x,y,v,buffer,startx,starty,endx,endy,VMEM
Уведомление об ошибке, когда конечные координаты будут меньше начальных или когда буфер
будет равен нулю*/
int put_vmem(int startx,int starty,int endx,int endy,char*buffer)
{
	int x,y;
	char*v;
	if(buffer==NULL) {fprintf(stderr,"Error: Invalid buffer size"); exit(1);}
	if(endx<=startx || endy<=starty) {fprintf(stderr,"Error: Invalid coordinates"); exit(1);}
		for(x=startx;x<=endx;x++) {
			for(y=starty;y<=endy;y++) {
				v=VMEM;
				v+=x*160+y*2;
				*v=*buffer;
				*v++;
				*buffer++;
				*v=*buffer;
				*buffer++;
			}
		}
	return 0;
}

int desktop(void)
{
	clear(0,0,48,79,0xB0,MAGENTA);
	return;
}

/*Системные переменные: x,y,startx,starty,endx,endy
Прикладные переменные: border1,border2,border3,border4,border5,border6,type
Уведомление об ошибке когда тип границы будет больше двух или меньше единицы*/
int border(int startx,int starty,int endx,int endy,char attrib,int type)
{
	int x, y,border1,border2,border3,border4,border5,border6;
	if(type>2 || type<1) {fprintf(stderr,"Error: Invalid bordertype"); exit(1);}
	switch (type){
		case 1:
			border1=0xDA; border2=0xBF;
			border3=0xC0; border4=0xD9;
			border5=0xC4; border6=0xB3;
			break;
		case 2:
			border1=0xC9; border2=0xBB;
			border3=0xC8; border4=0xBC;
			border5=0xCD; border6=0xBA;
			break;
	}
	for(x=startx;x<=endx;x++) {
		for(y=starty;y<=endy;y++) {
			if(x==startx||x==endx) {
				write_char(x,y,border5,attrib);
			}
			if(y==starty||y==endy) {
				write_char(x,y,border6,attrib);
			}
			if(x==startx && y==starty) {
				write_char(x,y,border1,attrib);
			}
			if(x==startx && y==endy) {
				write_char(x,y,border2,attrib);
			}
			if(x==endx && y==starty) {
				write_char(x,y,border3,attrib);
			}
			if(x==endx && y==endy) {
				write_char(x,y,border4,attrib);
			}
		}
	}
	return;
}

/*Системные переменные: startx,starty,endy,pfix,check,y
Прикладные переменные: name,attrib
Уведомление об ошибке когда конечные координаты будут меньше начальных или когда ширина окна
будет недостаточной для вмещения имени*/
int name(int startx,int starty,int endy,char*name,char attrib)
{
	int pfix=strlen(name);
	int check=endy+starty, y;
	if(endy<starty) {fprintf(stderr,"Error: Invalid name coordinates"); exit(1);}
	if((endy-starty)<(pfix+1)) {fprintf(stderr,"Error: Invalid name size"); exit(1);}
	if(check%2==1) check++;
	y=check/2-pfix/2;
	write_string(startx,y,name,attrib);
	return;
}

int get_key(void)
{
	_AH=0x00;
	geninterrupt(0x16);
	return _AX;
}

void goto_xy(int x,int y)
{
	_AH=0x02;
	_DL=y;
	_DH=x;
	_BH=0x00;
	geninterrupt(0x10);
}
