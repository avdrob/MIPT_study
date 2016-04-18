/*
  ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
  ³ This is an example how you can include SPR files into C programs   ³
  ³	with a extern color palette. The program shows, how you have to    ³
  ³	setup and install the color palette.                               ³
  ³								                                       ³
  ³                   Programmed with Turbo C++ 1.0                    ³
  ³                              								       ³
  ³                      (c) Markus Herbord 1994        		       ³
  ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/


#include<dos.h>
#include<stdio.h>
#include<process.h>
#include<stdlib.h>
#include<io.h>
#include<graphics.h>
#include<fcntl.h>
#include<string.h>
#include<alloc.h>
#include<conio.h>

extern int far Svga256_fdriver[];

int huge DetectVGA256()                                /* Select video mode */
{
  int Vid;
  Vid=0;                                      /* 320x200x256 is videomode 0 */
  return Vid;
}

void SetCol(unsigned char color, unsigned char r, unsigned char g, unsigned char b)
{
  outportb(0x3C8, color);     /* write the number of the color to port 3C8h */
  outportb(0x3C9, r);                              /* its red value to 3C9h */
  outportb(0x3C9, g);                                 /* also its green and */
  outportb(0x3C9, b);                                         /* blue value */
}


void main(void)
{
  int				Gd = DETECT, Gm, Drv,j,x, y, maxx;
  int				sprfile, palfile;
  unsigned long		size;
  unsigned char		rv, gv, bv, i;
  void				*pic;
  char 				*palfilename="cexample.pal";  /* this is the palette file */
  char 				*sprname="cexample.spr";          /* and this the picture */

  /* change this, if you would like to load another picture
	 the file must be in the same directory as the source, I
	 don't know why :-(                                      */



  installuserdriver("Svga256",DetectVGA256);          /* install driver  */
  initgraph(&Gd,&Gm,"");                              /* install graphic */


  if ((palfile=open(palfilename,O_RDONLY | O_BINARY))!=-1); /* open the file */
  {                                   /*! must be binary !*/
	j=0; i=0;
	do
	{
		rv=0; gv=0; bv=0;

		read(palfile, &rv, 1);             /* read the red value ( 1 byte ) */
		read(palfile, &gv, 1);           /* read the green value ( 1 byte ) */
		read(palfile, &bv, 1);            /* read the blue value ( 1 byte ) */

		SetCol(i, rv, gv, bv);                             /* set the color */

		j++;   /* next j (j is integer, don't use i, because it will always */
		i++;                                 /* be smaller or equal to 255) */
	}
	 while (j<=255);                              /* all 256 (0-255) colors */
   }

  size=0;
  sprfile=open(sprname,O_RDONLY);                          /* open the file */
  size = filelength(sprfile);                           /* how much memory? */
  if ((pic = malloc(size))!=NULL)                /* allocate memory for pic */
  {
     read(sprfile, pic, size);
	 putimage(0,0,pic, COPY_PUT);                             /* show image */
	 free(pic);
	 setcolor(1);
	 outtextxy(20,156,"  This is an example, how you can");
	 outtextxy(20,166,"   use sprites, created with the ");
	 setcolor(15);
	 outtextxy(20,180,"      Sprite Generator V1.0");
  }
  else
  {
     setcolor(15);
	 outtextxy(10,10,"Cannot load picture.");                   /* an error */
  }
  getch();
  closegraph();
}