/************************************************/
/* 						*/
/*      SuperVGA 65536 BGI driver defines	*/
/*		Copyright (c) 1993		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

extern int far _Cdecl Svga64k_fdriver[];

/* These are the currently supported modes */
#ifndef SVGA320x200x65536
#define	SVGA320x200x65536	0	/* 320x200x65536 HiColor VGA */
#define	SVGA640x350x65536	1	/* 640x350x65536 HiColor VGA */
#define	SVGA640x400x65536	2	/* 640x400x65536 HiColor VGA */
#define	SVGA640x480x65536	3	/* 640x480x65536 HiColor VGA */
#define	SVGA800x600x65536	4	/* 800x600x65536 HiColor VGA */
#endif

#ifndef XNOR_PUT
#define	XNOR_PUT	5
#define NOR_PUT		6
#define NAND_PUT	7
#define TRANS_COPY_PUT	8	/* Doesn't work with 16-color drivers */
#endif

#define RGB(r,g,b) ((r & 31)<<11) | ((g & 31)<<5) | (b & 31)

int RealDrawColor(int color)
{
  if (getmaxcolor() > 256) 
    setrgbpalette(1024,(color>>11)&31,(color>>5)&63,color&31);
  return(color);
}

int RealFillColor(int color)
{
  if (getmaxcolor() > 256)
    setrgbpalette(1025,(color>>11)&31,(color>>5)&63,color&31);
  return(color);
}

int RealColor(int color)
{
  if (getmaxcolor() > 256)
    setrgbpalette(1026,(color>>11)&31,(color>>5)&63,color&31);
  return(color);
}
