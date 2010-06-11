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

/*
 * M x M regions that image will be divided. 
 * We could use also 24, 18 and 12 as stated in the paper.
 */
#define M 32


int create_sub_images(unsigned char *, int, int, unsigned int, unsigned char *****);

#endif

