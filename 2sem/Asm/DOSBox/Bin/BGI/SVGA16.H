/************************************************/
/* 						*/
/*        SuperVGA 16 BGI driver defines	*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>

#ifndef _DAC16_
#define _DAC16_
typedef unsigned char DacPalette16[16][3];
#endif

extern int far Svga16_fdriver[];

/* These are the currently supported modes */
#ifndef SVGA320x200x16
#define SVGA320x200x16	0	/* 320x200x16 Standard EGA/VGA */
#define SVGA640x200x16	1	/* 640x200x16 Standard EGA/VGA */
#define SVGA640x350x16	2	/* 640x350x16 Standard EGA/VGA */
#define SVGA640x480x16	3	/* 640x480x16 Standard VGA */
#define SVGA800x600x16	4	/* 800x600x16 SuperVGA/VESA */
#define SVGA1024x768x16	5	/* 1024x768x16 SuperVGA/VESA */
#endif

#ifndef XNOR_PUT
#define	XNOR_PUT	5
#define NOR_PUT		6
#define NAND_PUT	7
#define TRANS_COPY_PUT	8	/* Doesn't work with 16-color drivers */
#endif
