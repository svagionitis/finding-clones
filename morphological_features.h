/* ############################################################################
Name           : morphological_features.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for morphological features.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  12/07/2010     Creation

############################################################################ */
#ifndef __MORPHOLOGICAL_FEATURES_H__
#define __MORPHOLOGICAL_FEATURES_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define ALLOW_PRINTF	FALSE

#define SMALL_NUMBER	0.000000001

#define MIN(a,b)	((a)<=(b)?(a):(b))
#define MAX(a,b)	((a)>=(b)?(a):(b))
#define MIN3(a,b,c)	((a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c)	((a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )

#define GREYSCALE(r, g, b)	((unsigned char)(0.2126 * (double)(r)) + \
                                 (unsigned char)(0.7152 * (double)(g)) + \
                                 (unsigned char)(0.0722 * (double)(b)))

#define PI  3.1415926535897932384626433832795
#define RT2 1.4142135623730950488016887242097 /* = sqrt(2.0) */

/*Define to change Radians2Degrees and vice versa*/
#define RADIANS_TO_DEGREES(X)	((180.0/PI)*X)
#define DEGREES_TO_RADIANS(X)	((PI/180.0)*X)


int morphological_feature_circularity_index(int *, double *, int, double *);
int morphological_feature_object_moment(double, double, int **, int, int, int, unsigned char *, double *);
int morphological_feature_calculate_centroids(int **, int, int, int, unsigned char *, double **);
int morphological_feature_central_moments(double, double, int **, int, int, int, unsigned char *, double *);
int morphological_feature_object_orientation(int **, int, int, int, unsigned char *, double *);
int morphological_feature_object_eccentricity(int **, int, int, int, unsigned char *, double *);
int morphological_feature_central_invariant_moments(double, double, int **, int, int, int, unsigned char *, double *);


#endif
