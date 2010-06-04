/* ############################################################################
Name           : pnm.h
Company        : ALCON 2009
Project        : Finding Clones
Programmer     : ALCON 2009
Revisor        : S. Vagionitis
Description    : Header file for PNM.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  02/06/2010     Creation

############################################################################ */
#ifndef __PNM_H__
#define __PNM_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


unsigned char *load_ppm(const char *filename, int *width, int *height);
unsigned char *load_pgm(const char *filename, int *width, int *height);

#endif
