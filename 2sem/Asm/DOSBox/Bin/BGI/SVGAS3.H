/************************************************/
/* 						*/
/*        SuperVGA S3 BGI driver defines	*/
/*		Copyright (c) 1992		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

extern int far _Cdecl SvgaS3_fdriver[];

/* These are the currently supported modes */
#ifndef SVGAS3_640x480x256
#define	SVGAS3_640x480x256	0	/* 640x480x256 S3 Svga */
#define	SVGAS3_800x600x256	1	/* 800x600x256 S3 Svga */
#define	SVGAS3_1024x768x256	2	/* 1024x768x256 S3 Svga */
#define	SVGAS3_800x600x16	3	/* 800x600x16 S3 Svga */
#define	SVGAS3_1024x768x16	4	/* 1024x768x16 S3 Svga */
#define	SVGAS3_1280x960x16	5	/* 1280x960x16 S3 Svga */
#define	SVGAS3_1280x1024x16	6	/* 1280x1024x16 S3 Svga */
#define	SVGAS3_640x480x32768	7	/* 640x480x32768 S3 HiColor Svga */
#endif

#ifndef XNOR_PUT
#define	XNOR_PUT	5
#define NOR_PUT		6
#define NAND_PUT	7
#define TRANS_COPY_PUT	8	/* Doesn't work with 16-color drivers */
#endif

#define RGB(r,g,b) ((r & 31)<<10) | ((g & 31)<<5) | (b & 31)

int RealDrawColor(int color)
{
  if (getmaxcolor() > 256) 
    setrgbpalette(1024,(color>>10)&31,(color>>5)&31,color&31);
  return(color);
}

int RealFillColor(int color)
{
  if (getmaxcolor() > 256)
    setrgbpalette(1025,(color>>10)&31,(color>>5)&31,color&31);
  return(color);
}

int RealColor(int color)
{
  if (getmaxcolor() > 256)
    setrgbpalette(1026,(color>>10)&31,(color>>5)&31,color&31);
  return(color);
}
