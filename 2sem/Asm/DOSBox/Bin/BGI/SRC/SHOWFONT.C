/*****************************************************************************
| This is a demonstration program for Microcosm Fonts for the Borland Graphics
| Library (BGI).
|
| (C) Copyright 1992-1995 Tom Wright, Microcosm, 619-488-4462
| internet e-mail wright@siggraph.org
|
| This program is legally protected. See the README file for details.
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <conio.h>

typedef unsigned char uchar;

// for your own use, uncomment the following line and adjust as needed
// #define  BGI_DIRECTORY  "c:\\borlandc\\bgi"

static char *style[] =
   { "Regular Roman Font", "Bold Font", "Itallic Font", "Bold Itallic Font" };
static int box[] = { 395, 260, 395, 290, 600, 290, 600, 260 };
static int nBlink = 3;

void bzzt() { sound(32); delay(32); nosound(); }

void checkerrors()
   {
   int   code;

   code = graphresult();
   if (code!=grOk)
      {
      outtextxy(10, 460, grapherrormsg(code));
      getch();
      cleardevice();
      closegraph();
      exit (fprintf(stderr, "error = %s\n", grapherrormsg(code))!=0);
      }
   }

void showFont(char *name)
   {
   int      i, j, fontHandle;
   uchar    ascii[3], message[30];;

   settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
   setcolor(CYAN);
   outtextxy(10, 20, "Microcosm BGI Font Demonstration Program");
   outtextxy(10, 40,
      "(C) Copyright 1992-1995 Tom Wright, Microcosm, 619-488-4462");
   outtextxy(10, 60, "internet e-mail wright@siggraph.org");
   outtextxy(400, 180, name);
   outtextxy(400, 200, "Microcosm Helvetica-like");
   i = (name[1]=='B' || name[1]=='b')+
      2*(strchr(&name[1], 'I')!=NULL || strchr(&name[1], 'i')!=NULL);
   outtextxy(400, 220, style[i]);
   sprintf(message, "CapHeight = %2d Pixels", name[3]-(name[3]<'A'?  '0': 55));
   outtextxy(400, 240, message);
   outtextxy(400, 300, "Register and get");
   outtextxy(400, 320, "245 BGI Fonts!");
   outtextxy(400, 340, "Send $29.95 to");
   outtextxy(400, 360, "Microcosm");
   outtextxy(400, 380, "819 Devon Ct.");
   outtextxy(400, 400, "San Diego, CA 92109");
   outtextxy(400, 420, "CA residents add 7% tax");
   outtextxy(400, 440, "See README file for details.");
   for (i=0; i<13; i++)
      {
      sprintf(ascii, "%2X", 32+(i>5? i+1 : i)*16);
      outtextxy(10, 195+i*20, ascii);
      }
   for (j=0; j<16; j++)
      {
      sprintf(ascii, "%1X", j);
      outtextxy(40+20*j, 170, ascii);
      }
   fontHandle = installuserfont(name);
   checkerrors();
   settextstyle(fontHandle, HORIZ_DIR, 0);
   setusercharsize(1, 1, 1, 1);
   checkerrors();
   setcolor(WHITE);
   outtextxy(10, 120, "The quick brown fox jumped over the lazy dog.");
   outtextxy(10, 140, "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.");
   ascii[1] = '\0';
   for (i=0; i<13; i++)
      for (j=0; j<16; j++)
         {
         ascii[0] = 32+(i>5? i+1 : i)*16+j;
         outtextxy(40+20*j, 190+i*20, ascii);
         }
   bzzt();
// settextstyle(DEFAULT_FONT, HORIZ_DIR, 0);
   for (i=0; i<=nBlink; i++) /* I hope you don't find this too offensive. */
      {
      setcolor(GREEN);
      outtextxy(400, 270, "Just 10¢ a font!");
      if (i<nBlink)
         {
         delay(200); if (kbhit()) break; delay(200); if (kbhit()) break;
         setcolor(BLACK);
         fillpoly(4, box);
         delay(200); if (kbhit()) break; delay(200); if (kbhit()) break;
         }
      }
   if (nBlink)
      nBlink--;
   setcolor(WHITE);
   if (getch()==27)                    // 27 == Esc
      { closegraph(); exit(1); }
   cleardevice();
   }

int main(int argc, char *argv[])
   {
   int   i;
   int   gdriver = DETECT, gmode;
   char  *name = "H--C";

#ifdef   BGI_DIRECTORY
   initgraph(&gdriver, &gmode, BGI_DIRECTORY);
#else
   // use of linked driver allows demo to run without knowing BGI location
   if (registerbgidriver(EGAVGA_driver)<0)
      return fprintf(stderr, "registerbgidriver failed\n")!=0;
   initgraph(&gdriver, &gmode, "");
#endif
   if (graphresult()!=grOk)
      return fprintf(stderr, "%s: initgraph failed\n", argv[0])!=0;
   setfillstyle(SOLID_FILL, BLACK);
   if (argc>1)
      strncpy(name, argv[1], 4);
   for (i=12; i>5; i--)
      {
      sprintf(&name[3], "%1X", i);
      showFont(name);
      }
   closegraph();
   return 0;
   }
