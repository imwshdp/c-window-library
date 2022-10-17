#if !defined (_WINDOW_H_)
#define _WINDOW_H_

#define LEFT 0x14B
#define RIGHT 0x14D
#define UP 0x148
#define DOWN 0x150
#define PGUP 0x149
#define PGDN 0x151
#define HOME 0x147
#define END 0x14F
#define INSERT 0x152
#define DEL 0x153
#define F1 0x13B
#define F2 0x13C
#define F3 0x13D
#define F4 0x13E
#define F5 0x13F
#define F6 0x140
#define F7 0x141
#define F8 0x142
#define F9 0x143
#define F10 0x144
#define F11 0x157
#define F12 0x158
#define BKSP 0x08
#define ENTER 0x0D
#define ESC 0x1B

typedef struct {
	int startx,starty,endx,endy;
	int curx,cury;
	int attrib;
	char name[12];
	char*p;
	int type;
	int active;
} window_t;

int window_getkey(void);
int window_xy(window_t*w, int x,int y);
void window_putchar(window_t*w ,char ch);
int window_gets(window_t*w,int len);
window_t* window_create(char*name,int startx,int starty,int endx,int endy,int attrib,int type);
void window_open(window_t*w);
void window_close (window_t*w);
void window_destroy(window_t*w);
#endif
