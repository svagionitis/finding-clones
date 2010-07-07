/* ############################################################################
Name           : color_features.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for color features.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  07/07/2010     Creation

############################################################################ */
#ifndef __COLOR_FEATURES_H__
#define __COLOR_FEATURES_H__

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

#define GREYSCALE(r, g, b)	((unsigned char)(0.2126 * (double)(r)) + \
                                 (unsigned char)(0.7152 * (double)(g)) + \
                                 (unsigned char)(0.0722 * (double)(b)))



int color_feature_mean(int **, int, int, int, int *, unsigned char *, double *);
int color_feature_standard_deviation(int **, int, int, int, int *, unsigned char *, double *, double *);
int color_feature_skewness(int **, int, int, int, int *, unsigned char *, double *, double *);
int color_feature_kurtosis(int **, int, int, int, int *, unsigned char *, double *, double *);

#endif
