/* ############################################################################
Name           : alg_lvl1.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for alg_lvl1.c.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */
#ifndef __ALG_LVL1_H__
#define __ALG_LVL1_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define PI  		3.1415926535897932384626433832795
#define RAD(x)		((x)*PI/180.0)
#define MIN(a,b)	((a)<=(b)?(a):(b))
#define MAX(a,b)	((a)>=(b)?(a):(b))
#define MIN3(a,b,c)	((a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c)	((a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )

#define GREYSCALE1(r, g, b)	((0.2126 * (r)) + (0.7152 * (g)) + (0.0722 * (b)))
#define GREYSCALE2(r, g, b)	((0.2989 * (r)) + (0.5870 * (g)) + (0.1140 * (b)))

/*
 * M x M regions that image will be divided. 
 * We could use also 24, 18 and 12 as stated in the paper.
 */
#define M 32


int create_sub_images(unsigned char *, int, int, unsigned int, unsigned char *****);

#endif

