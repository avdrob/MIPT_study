/************************************************/
/* 						*/
/*        Tweaked 256 BGI driver defines	*/
/*		Copyright (c) 1991		*/
/*	    Jordan Hargraphix Software		*/
/*						*/
/************************************************/

#include <dos.h>

#ifndef _DAC256_
#define _DAC256_
typedef unsigned char DacPalette256[256][3];
#endif

extern int far _Cdecl Twk256_fdriver[];

/* These are the currently supported modes */
#ifndef TWK320x400x256
#define	TWK320x400x256		0
#define	TWK320x480x256		1
#define	TWK360x480x256		2
#define	TWK376x564x256		3
#define	TWK400x564x256		4
#define	TWK400x600x256		5
#endif

#ifndef XNOR_PUT
#define	XNOR_PUT	5
#define NOR_PUT		6
#define NAND_PUT	7
#define TRANS_COPY_PUT	8	/* Doesn't work with 16-color drivers */
#endif
