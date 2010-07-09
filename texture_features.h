/* ############################################################################
Name           : texture_features.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for texture features.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  08/07/2010     Creation

############################################################################ */
#ifndef __TEXTURE_FEATURES_H__
#define __TEXTURE_FEATURES_H__

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



int glcm(unsigned char, int **, int, int, int, unsigned char *, double ***);
int texture_feature_angular_second_moment(double ***, int, double *);
int texture_feature_contrast(double ***, int, double *);
int texture_feature_correlation(double ***, int, double *);
int texture_feature_variance(double ***, int, double *);
int texture_feature_inverse_diff_moment(double ***, int, double *);




#endif
