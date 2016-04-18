/* bgi.h */

#ifndef _BGI_H
#define _BGI_H

#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN extern
#endif
#endif

EXTERN void ATT_driver (void);
EXTERN void CGA_driver (void);
EXTERN void DEBUG_driver (void);
EXTERN void EGAVGA_driver (void);
EXTERN void HERC_driver (void);
EXTERN void IBM8514_driver (void);
EXTERN void PC3270_driver (void);
EXTERN void VGA256_driver (void);
EXTERN void BOLD_font (void);
EXTERN void EURO_font (void);
EXTERN void GOTH_font (void);
EXTERN void LCOM_font (void);
EXTERN void LITT_font (void);
EXTERN void RUSS_font (void);
EXTERN void SANS_font (void);
EXTERN void SCRI_font (void);
EXTERN void SIMP_font (void);
EXTERN void TRIP_font (void);
EXTERN void TSCR_font (void);

EXTERN far ATT_driver_far[];
EXTERN far CGA_driver_far[];
EXTERN far DEBUG_driver_far[];
EXTERN far EGAVGA_driver_far[];
EXTERN far HERC_driver_far[];
EXTERN far IBM8514_driver_far[];
EXTERN far PC3270_driver_far[];
EXTERN far VGA256_driver_far[];
EXTERN far BOLD_font_far[];
EXTERN far EURO_font_far[];
EXTERN far GOTH_font_far[];
EXTERN far LCOM_font_far[];
EXTERN far LITT_font_far[];
EXTERN far RUSS_font_far[];
EXTERN far SANS_font_far[];
EXTERN far SCRI_font_far[];
EXTERN far SIMP_font_far[];
EXTERN far TRIP_font_far[];
EXTERN far TSCR_font_far[];


#endif