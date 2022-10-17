#if !defined (_BASE_H_)
#define _BASE_H_

#define VMEM (char*)0xB8000000L
#define BLACK 0x0000
#define BLUE 0x0001
#define GREEN 0x0002
#define CYAN 0x0003
#define RED 0x0004
#define MAGENTA 0x0005
#define BROWN 0x0006
#define LIGHTGRAY 0x0007
#define DARKGRAY 0x0008
#define LIGHTBLUE 0x0009
#define LIGHTGREEN 0x000A
#define LIGHTCYAN 0x000B
#define LIGHTRED 0x000C
#define LIGHTMAGENTA 0x000D
#define YELLOW 0x000E
#define WHITE 0x000F
#define BGBLACK 0x0000
#define BGBLUE 0x0010
#define BGGREEN 0x0020
#define BGCYAN 0x0030
#define BGRED 0x0040
#define BGMAGENTA 0x0050
#define BGBROWN 0x0060
#define BGLIGHTGRAY 0x0070
#define BGDARKGRAY 0x0080
#define BGLIGHTBLUE 0x0090
#define BGLIGHTGREEN 0x00A0
#define BGLIGHTCYAN 0x00B0
#define BGLIGHTRED 0x00C0
#define BGLIGHTMAGENTA 0x00D0
#define BGYELLOW 0x00E0
#define BGWHITE 0x00F0

int write_char(int x,int y,char ch,char attrib);
int write_string(int x,int y,char* string,char attrib);
int clear(int startx,int starty,int endx,int endy,char ch,char attrib);
char* create_buffer(int startx,int starty,int endx,int endy);
int get_vmem(int startx,int starty,int endx,int endy,char*buffer);
int put_vmem(int startx,int starty,int endx,int endy,char*buffer);
int desktop(void);
int border(int startx,int starty,int endx,int endy,char attrib,int type);
int name(int startx,int starty,int endy,char*name,char attrib);
int get_key(void);
void goto_xy(int x,int y);
#endif
