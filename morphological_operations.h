/* ############################################################################
Name           : morphological_operations.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for morphological operations.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  15/07/2010     Creation

############################################################################ */
#ifndef __MORPHOLOGICAL_OPERATIONS_H__
#define __MORPHOLOGICAL_OPERATIONS_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define MIN(a,b)	((a)<=(b)?(a):(b))
#define MAX(a,b)	((a)>=(b)?(a):(b))
#define MIN3(a,b,c)	((a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c)	((a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )


int transform_1D_to_2D_Binary_Images(unsigned char *, int, int, unsigned char **);

int manhattan_distance_dilate_2D(unsigned int **, int, int);
int manhattan_distance_dilate_1D(unsigned int *, int, int);

int manhattan_distance_erode_2D(unsigned int **, int, int);
int manhattan_distance_erode_1D(unsigned int *, int, int);

int dilate_2D(unsigned char **, int, int, unsigned int);
int dilate_1D(unsigned char *, int, int, unsigned int);

int erode_2D(unsigned char **, int, int, unsigned int);
int erode_1D(unsigned char *, int, int, unsigned int);

int opening_1D(unsigned char *, int, int, unsigned int);
int closing_1D(unsigned char *, int, int, unsigned int);


#endif

