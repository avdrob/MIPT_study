
#ifndef __WINBGI_H
#define __WINBGI_H

#define far
#define huge
#define random(range)  ( rand() % (range) )

#ifndef __COLORS
#define __COLORS

enum colors
	{ 
	BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY, DARKGRAY, 
	LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, YELLOW, WHITE
	};

#endif

enum write_modes 
	{ 
	COPY_PUT, XOR_PUT, OR_PUT, AND_PUT, NOT_PUT 
	};

enum line_styles 
	{ 
	SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE, USERBIT_LINE
	};

enum fill_styles 
	{ 
	EMPTY_FILL, SOLID_FILL, LINE_FILL, LTSLASH_FILL, SLASH_FILL, BKSLASH_FILL,
	LTBKSLASH_FILL, HATCH_FILL, XHATCH_FILL, INTERLEAVE_FILL, WIDE_DOT_FILL, 
	CLOSE_DOT_FILL, USER_FILL
	};

enum text_directions 
	{ 
	HORIZ_DIR, VERT_DIR
	};

enum font_types 
	{ 
	DEFAULT_FONT, TRIPLEX_FONT, SMALL_FONT, SANSSERIF_FONT, GOTHIC_FONT 
	};

#define LEFT_TEXT					0
#define CENTER_TEXT					1
#define RIGHT_TEXT					2
#define BOTTOM_TEXT					0
#define TOP_TEXT					2

#define NORM_WIDTH					1
#define THICK_WIDTH					3

#define DOTTEDLINE_LENGTH			2
#define CENTRELINE_LENGTH			4

#define USER_CHAR_SIZE				0
#define MAXCOLORS					15 

#define CLIP_ON						1
#define CLIP_OFF					0

#define TOP_ON						1
#define TOP_OFF					    0

enum graphics_errors 
	{
	grOk				=  0,
	grNoInitGraph		= -1,
	grNotDetected		= -2,
	grFileNotFound		= -3,
	grInvalidDriver		= -4,
	grNoLoadMem			= -5,
	grNoScanMem			= -6,
	grNoFloodMem		= -7,
	grFontNotFound		= -8,
	grNoFontMem			= -9,
	grInvalidMode		= -10,
	grError				= -11,
	grIOerror			= -12,
	grInvalidFont		= -13,
	grInvalidFontNum	= -14,
	grInvalidDeviceNum	= -15,
	grInvalidVersion	= -18
	};


// Graphics drivers constants, includes X11 which is particular to XBGI

#define DETECT				0
#define CGA					1
#define MCGA				2
#define EGA					3
#define EGA64				4
#define EGAMONO				5
#define IBM8514				6
#define HERCMONO			7
#define ATT400				8
#define VGA					9
#define PC3270				10

// Graphics modes constants

#define CGAC0				0
#define CGAC1				1
#define CGAC2				2
#define CGAC3				3
#define CGAHI				4

#define MCGAC0				0
#define MCGAC1				1
#define MCGAC2				2
#define MCGAC3				3
#define MCGAMED				4
#define MCGAHI				5

#define EGALO				0
#define EGAHI				1

#define EGA64LO				0
#define EGA64HI				1

#define EGAMONOHI			3
#define HERCMONOHI			0

#define ATT400C0			0
#define ATT400C1			1
#define ATT400C2			2
#define ATT400C3			3
#define ATT400MED			4
#define ATT400HI			5

#define VGALO				0
#define VGAMED				1
#define VGAHI				2
#define VGAMAX              3

#define PC3270HI			0

#define IBM8514LO			0
#define IBM8514HI			1

typedef struct arccoordstype 
	{
	int x;
	int y;
	int xstart;
	int ystart;
	int xend;
	int yend;
	} 
	arccoordstype;

typedef char fillpatterntype[8];

typedef struct fillsettingstype 
	{
	int pattern;
	int color;
	} 
	fillsettingstype;

typedef struct linesettingstype 
	{
	int linestyle;
	unsigned upattern;
	int thickness;
	} 
	linesettingstype;

typedef struct palettetype 
	{
	unsigned char size;
	signed char colors[16];
	} 
	palettetype;

typedef struct textsettingstype 
	{
	int font;	
	int direction;  
	int charsize;
	int horiz;
	int vert;
	} 
	textsettingstype;

typedef struct viewporttype 
	{
	int left;
	int top;
	int right;
	int bottom;
	int clip;
	} 
	viewporttype;

#if defined (__cplusplus) 
extern "C" {
#endif

//
// Setting this variable to 0 increase speed of drawing but
// correct redraw is not possible. By default this variable is initialized by 1
//

extern int bgiemu_handle_redraw;

//
// Default mode choosed by WinBGI if DETECT value is specified for 
// device parameter of initgraph (). Default value is VGAMAX which
// cause creation of maximized window  (resolution depends on display mode)
//

extern int bgiemu_default_mode;

void  _graphfreemem (void* ptr, unsigned size);
void* _graphgetmem (unsigned size);
void  arc (int x, int y, int stangle, int endangle, int radius);
void  bar (int x0, int y0, int x1, int y1);
void  bar3d (int x0, int y0, int x1, int y1, int depth, int topFlag);
void  circle (int x, int y, int r);
void  cleardevice();
void  clearviewport();
void  closegraph();
void  detectgraph (int* driver, int* mode);
void  drawpoly (int npoints, int* points);
void  ellipse (int x, int y, int stangle, int endangle, int rx, int ry);
void  fillellipse (int x, int y, int rx, int ry);
void  fillpoly (int npoints, int* points);
void  floodfill (int x, int y, int border);
void  getarccoords (struct arccoordstype* info);
void  getaspectratio (int* x, int* y);
int   getbkcolor();
int   getcolor();
struct palettetype* getdefaultpalette();
char* getdrivername();
void  getfillpattern (char* pattern);
void  getfillsettings (struct fillsettingstype* info);
int   getgraphmode();
void  getimage (int x0, int y0, int x1, int y1, void* image);
void  getlinesettings (struct linesettingstype* info);
int   getmaxcolor();
int   getmaxmode();
int   getmaxx();
int   getmaxy();
char* getmodename (int mode);
void  getmoderange (int driver, int* minmode, int* maxmode);
void  getpalette (struct palettetype* info);
int   getpalettesize();
unsigned getpixel (int x, int y);
void  gettextsettings (struct textsettingstype* info);
void  getviewsettings (struct viewporttype* info);
int   getx();
int   gety();
void  graphdefaults();
char* grapherrormsg (int err);
int   graphresult();
unsigned imagesize (int x0, int y0, int x1, int y1);
void  initgraph (int* driver, int* mode, const char* drvpath);
int   installuserdriver (const char* drivername, void* autodetectFn);
int   installuserfont (const char* fontname);
void  line (int x0, int y0, int x1, int y1);
void  linerel (int dx, int dy);
void  lineto (int x, int y);
void  moverel (int dx, int dy);
void  moveto (int x, int y);
void  outtext (const char* text);
void  outtextxy (int x, int y, const char* text);
void  pieslice (int x, int y, int stangle, int endangle, int r);
void  putimage (int x, int y, void* image, int op);
void  putpixel (int x, int y, int color);
void  rectangle (int x0, int y0, int x1, int y1);
int   registerbgidriver (void* driverfunc);
int   registerbgifont (void* fontfunc);
void  restorecrtmode();
void  sector (int x, int y, int stangle, int endangle, int rx, int ry);
void  setactivepage (int page);
void  setallpalette (struct palettetype* info);
void  setaspectratio (int x, int y);
void  setbkcolor (int color);
void  setcolor (int color);
void  setfillpattern (const char* pattern, int color);
void  setfillstyle (int type, int color);
unsigned setgraphbufsize (unsigned size);
void  setgraphmode (int mode);
void  setlinestyle (int type, unsigned pattern, int thickness);
void  setpalette (int index, int contents);
void  setrgbpalette (int color, int r, int g, int b);
void  settextjustify (int hor, int ver);
void  settextstyle (int font, int direction, int size);
void  setusercharsize (int xmul, int xdiv, int ymul, int ydiv);
void  setviewport (int x0, int y0, int x1, int y1, int clip);
void  setvisualpage (int page);
void  setwritemode (int mode);
int   textheight (const char* text);
int   textwidth (const char* text);

int   getch();
int   kbhit();		
void  delay (unsigned msec);

#if defined (__cplusplus) 
};
#endif

#endif
