#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dos.h>
#include<base.h>
#include<window.h>

window_t*opcount[500]; /*массив для того, чтобы не дать закрыть не открытое окно*/
int onumber=0;
window_t*clcount[500]; /*массив для того, чтобы не дать уничтожить не закрытое окно*/
int cnumber=0;

int window_getkey(void)
{
	int code;
	code=get_key();
	if(code%0x100==0) {
		code=code/0x100+0x100;
	} else {
		code=code%0x100;
	}
	return code;
}

int window_xy(window_t*w, int x,int y)
{
	int endx=w->endx-w->startx-2;
	int endy=w->endy-w->starty-2;

	if(!w->active) {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
	if(y<0 && x<=0) return 1;
	if((y>endy && x==endx) || x>endx) return 1;
	if(y>endy && x<endx) {
		w->cury=0; w->curx++;
	} else if(y<0 && x>0) {
		w->curx--; w->cury=endy+1;
	} else {
			w->cury=y; w->curx=x;
	}
	goto_xy(w->startx+w->curx+1,w->starty+w->cury+1);
	return 0;
}

void window_putchar(window_t*w ,char ch)
{
	if(!w->active) {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
	if(ch=='\n') {
		w->curx++;
		w->cury=0;
	} else {
		write_char(w->curx+w->startx+1, w->cury+w->starty+1, ch, w->attrib);
	}
	goto_xy(w->curx+w->startx+1,w->cury+w->starty+1);
}

int window_gets(window_t*w,int len)
{
	int endy=w->endy-w->starty-2;
	int initialx=w->curx, initialy=w->cury, count=0;
	char*buffer4window;
	int ch;
	int flag4begin=1,flag4end=0;
	if(!w->active) {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
	buffer4window=create_buffer(w->curx,w->cury,w->endx-1,w->endy-1);
	get_vmem(w->curx,w->cury,w->endx-1,w->endy-1,buffer4window);
	while(1) {
		if(count==len) {ch ='\0'; exit(0);}
		ch=window_getkey();
		if(ch>0xFF && ch!=F10) continue;
		if(flag4begin==0) break;
			switch(ch) {
				case F10:
					ch='\0';
					free(buffer4window);
					flag4begin=0;
					break;
				case ESC:
					put_vmem(initialx,initialy,w->endx-1,w->endy-1,buffer4window);
					window_xy(w,initialx,initialy);
					w->curx=initialx;
					w->cury=initialy;
					count=0;
					break;
				case ENTER:
					ch ='\0';
					free(buffer4window);
					flag4begin=0;
					break;
				case BKSP:
					if(w->cury==0) {
						if(w->curx==0) continue;
						else {
							window_xy(w,w->curx-1,endy);
							window_putchar(w,' ');
							count--;
						}
					} else {
						if(w->cury==w->endy-w->starty-2 && w->curx==w->endx-w->startx-2 && flag4end==0) {
							window_putchar(w,' ');
							flag4end=1;
						} else {
							window_xy(w,w->curx,w->cury-1);
							window_putchar(w,' ');
							count--;
							flag4end=0;
						}
					}
					break;
				default:
				if(w->cury==endy) window_xy(w,w->curx,endy);
					window_putchar(w,(char)ch);
					window_xy(w,w->curx,w->cury+1);
					count++;
					flag4end=0;
			}
	}
	return 0;
}

/*Системные переменные: startx,starty,endx,endy,p
Прикладные переменные: attrib,type,name*/
window_t* window_create(char*name,int startx,int starty,int endx,int endy,int attrib,int type)
{
	window_t* w=malloc(sizeof(window_t));
	w->startx=startx;
	w->starty=starty;
	w->endx=endx;
	w->endy=endy;
	w->attrib=attrib;
	strcpy(w->name,name);
	w->p=create_buffer(startx,starty,endx,endy);
	w->type=type;
	w->active=0;
	return w;
}

/*Системные переменные: startx,starty,endx,endy,p,w,opcount,onumber
Прикладные переменные: name,attrib,type
Уведомление об ошибке когда открываемое окно не будет создано*/
void window_open(window_t*w)
{
	if(w==NULL) {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
	opcount[onumber]=w;
	onumber++;
	get_vmem(w->startx,w->starty,w->endx,w->endy,w->p);
	clear(w->startx,w->starty,w->endx,w->endy,' ',w->attrib);
	border(w->startx,w->starty,w->endx,w->endy,w->attrib,w->type);
	name(w->startx,w->starty,w->endy,w->name,w->attrib);
	w->curx=0;
	w->cury=0;
	w->active=1;
}

/*Системные переменные: startx,starty,endx,endy,w,opcount,onumber,clcount,cnumber
Прикладные переменные: name,attrib,type
Уведомление об ошибке когда закрываемое окно не будет открыто заранее*/
void window_close (window_t*w)
{
	if(opcount[onumber-1]==w) {
		put_vmem(w->startx,w->starty,w->endx,w->endy,w->p);
		clcount[cnumber]=w;
		cnumber++;
		w->active=0;
	} else {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
}

/*Системные переменные: w,clcount,cnumber
Уведомление об ошибке когда уничтожаемое окно не будет закрыто заранее*/
void window_destroy(window_t*w)
{
	if(clcount[cnumber-1]==w) {
		free(w->p);
		free(w);
	} else {fprintf(stderr,"Error: Invalid window operation"); exit(1);}
}
