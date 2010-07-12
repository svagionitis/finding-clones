/* ############################################################################
Name           : morphological_features.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : morphological features

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  08/07/2010     Creation

############################################################################ */

#define __MORPHOLOGICAL_FEATURES_C__
#include "morphological_features.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <string.h>


/* ############################################################################
Name           : morphological_feature_circularity_index
Description    : Calculate the circularity index which is a morphological 
                 feature. Values range from 1, for a perfect circle, to 0 
                 for a line. It is useful to give an impression of elongation 
                 as well as roughness of the object's shape. Other significant 
                 characteristics of this feature are that it is invariant to 
                 scale, translation and rotation.

Arguments      Type             Description
============== ================ ===============================================
area(IN)       int  *           Area covered by the objects.
perim(IN)      double *         Perimeter of the objects.
n_object(IN)   int              Number of objects from mask.
circ(OUT)      double *         Circularity index of objects.

Return Values                   Description
=============================== ===============================================
TRUE                            If everything is fine.

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================
i              unsigned in      General purpose index.

############################################################################ */
int morphological_feature_circularity_index(int *area, double *perim, int n_object, double *circ)
{
unsigned int i = 0;

printf("Circularity index:\n");
/* calcuate cirularity index */
for (i = 0; i < n_object; i++){
	circ[i] = 4.0*PI * area[i] / (perim[i]*perim[i]);
	printf("%d %.3f\n", i, circ[i]);
	}

return TRUE;
}

/*moment of order (p+q)*/ 
int morphological_feature_object_moment(double p, double q, int **obj_id, int width, int height, int n_object, unsigned char *image, double *moment)
{
unsigned int i = 0, j = 0;

double *sum = NULL;
sum = (double *)malloc(n_object * sizeof(double));
if (sum == NULL){
	printf("morphological_feature_object_moment:Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(sum, 0, sizeof(n_object * sizeof(double)));

for (i = 0; i < height; i++) {
	double a = pow(i, p);
	for (j = 0; j < width; j++) {
		double b = pow(j, q);
		if (!obj_id[i][j])
			continue;

		register unsigned int idx = (j + i*width)*3;
		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);

		moment[(obj_id[i][j] - 1)] += a * b * (double)val;
		sum[(obj_id[i][j] - 1)] += (double)val;
		}
	}

printf("Moment(%.0f, %.0f) :\n", p, q);
/* calcuate cirularity index */
for (i = 0; i < n_object; i++){
	printf("%d %.3f\n", i, moment[i]);
	}


free(sum);
return TRUE;
}

int morphological_feature_calculate_centroids(int **obj_id, int width, int height, int n_object, unsigned char *image, double **centroids)
{
unsigned int i = 0;

double *moment00 = NULL;
moment00 = (double *)malloc(n_object * sizeof(double));
if (moment00 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(moment00, 0, (n_object * sizeof(double)));

double *moment10 = NULL;
moment10 = (double *)malloc(n_object * sizeof(double));
if (moment10 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(moment10, 0, (n_object * sizeof(double)));

double *moment01 = NULL;
moment01 = (double *)malloc(n_object * sizeof(double));
if (moment01 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(moment01, 0, (n_object * sizeof(double)));

morphological_feature_object_moment(0.0, 0.0, obj_id, width, height, n_object, image, moment00);
morphological_feature_object_moment(1.0, 0.0, obj_id, width, height, n_object, image, moment10);
morphological_feature_object_moment(0.0, 1.0, obj_id, width, height, n_object, image, moment01);

printf("Centroids :\n");
for (i = 0; i < n_object; i++){
	centroids[i][0] = moment10[i] / moment00[i];
	centroids[i][1] = moment01[i] / moment00[i];
	printf("%d %.3f %.3f\n", i, centroids[i][0], centroids[i][1]);
	}


free(moment00);
free(moment10);
free(moment01);

return TRUE;
}


int morphological_feature_central_moments(double p, double q, int **obj_id, int width, int height, int n_object, unsigned char *image, double *central_moment)
{
unsigned int i = 0, j = 0;

double **centroid = NULL;
centroid = (double **)malloc(n_object*sizeof(double *));
if (centroid == NULL){
	printf("Could not allocate %d bytes.\n", n_object*sizeof(double *));
	exit(0);
	}
else{
	for (i = 0; i < n_object; i++){
		centroid[i] = (double *)malloc(2*sizeof(double));
		if (centroid[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", 2*sizeof(double), i);
			exit(0);
			}
		else{
			centroid[i][0] = 0.0;
			centroid[i][1] = 0.0;
			}
		}
	}

morphological_feature_calculate_centroids(obj_id, width, height, n_object, image, centroid);

for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
		if (!obj_id[i][j])
			continue;

		double a = pow((i - centroid[(obj_id[i][j] - 1)][0]), p);
		double b = pow((j - centroid[(obj_id[i][j] - 1)][1]), q);

		register unsigned int idx = (j + i*width)*3;
		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);

		central_moment[(obj_id[i][j] - 1)] += a * b * (double)val;
		}
	}

printf("Central moment(%.0f, %.0f) :\n", p, q);
/* calcuate cirularity index */
for (i = 0; i < n_object; i++){
	printf("%d %.3f\n", i, central_moment[i]);
	}


for (i = 0; i < n_object; i++)
	free(centroid[i]);
free(centroid);

return TRUE;
}

int morphological_feature_object_orientation(int **obj_id, int width, int height, int n_object, unsigned char *image, double *object_orientation)
{
unsigned int i = 0;

double *mi00 = NULL;
mi00 = (double *)malloc(n_object * sizeof(double));
if (mi00 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi00, 0, (n_object * sizeof(double)));

double *mi11 = NULL;
mi11 = (double *)malloc(n_object * sizeof(double));
if (mi11 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi11, 0, (n_object * sizeof(double)));

double *mi20 = NULL;
mi20 = (double *)malloc(n_object * sizeof(double));
if (mi20 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi20, 0, (n_object * sizeof(double)));

double *mi02 = NULL;
mi02 = (double *)malloc(n_object * sizeof(double));
if (mi02 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi02, 0, (n_object * sizeof(double)));


morphological_feature_central_moments(0.0, 0.0, obj_id, width, height, n_object, image, mi00);
morphological_feature_central_moments(1.0, 1.0, obj_id, width, height, n_object, image, mi11);
morphological_feature_central_moments(2.0, 0.0, obj_id, width, height, n_object, image, mi20);
morphological_feature_central_moments(0.0, 2.0, obj_id, width, height, n_object, image, mi02);


printf("Object orientation:\n");
for (i = 0; i < n_object; i++){
	object_orientation[i] = 0.5*atan((2.0*(mi11[i] / mi00[i])) / ((mi20[i] / mi00[i]) - (mi02[i] / mi00[i])));
	printf("%d %.3f %.3f\n", i, object_orientation[i], RADIANS_TO_DEGREES(object_orientation[i]));
	}

free(mi00);
free(mi11);
free(mi20);
free(mi02);

return TRUE;
}

int morphological_feature_object_eccentricity(int **obj_id, int width, int height, int n_object, unsigned char *image, double *object_eccentricity)
{
unsigned int i = 0;

double *mi00 = NULL;
mi00 = (double *)malloc(n_object * sizeof(double));
if (mi00 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi00, 0, (n_object * sizeof(double)));

double *mi20 = NULL;
mi20 = (double *)malloc(n_object * sizeof(double));
if (mi20 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi20, 0, (n_object * sizeof(double)));

double *mi02 = NULL;
mi02 = (double *)malloc(n_object * sizeof(double));
if (mi02 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi02, 0, (n_object * sizeof(double)));


morphological_feature_central_moments(0.0, 0.0, obj_id, width, height, n_object, image, mi00);
morphological_feature_central_moments(2.0, 0.0, obj_id, width, height, n_object, image, mi20);
morphological_feature_central_moments(0.0, 2.0, obj_id, width, height, n_object, image, mi02);


printf("Object eccentricity:\n");
for (i = 0; i < n_object; i++){
	double mei02 = (mi02[i] / mi00[i]);
	double mei20 = (mi20[i] / mi00[i]);
	object_eccentricity[i] = fabs((mei02 - mei20) / (mei02 + mei20));
	printf("%d %.3f\n", i, object_eccentricity[i]);
	}

free(mi00);
free(mi20);
free(mi02);

return TRUE;
}


int morphological_feature_central_invariant_moments(double p, double q, int **obj_id, int width, int height, int n_object, unsigned char *image, double *central_invariant_moment)
{
unsigned int i = 0;

if (p+q < 2.0){
	printf("morphological_feature_central_invariant_moments: p+q < 2!!! p+q should be >=2\n");
	return FALSE;
	}
	
double *mi00 = NULL;
mi00 = (double *)malloc(n_object * sizeof(double));
if (mi00 == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mi00, 0, (n_object * sizeof(double)));

double *mipq = NULL;
mipq = (double *)malloc(n_object * sizeof(double));
if (mipq == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mipq, 0, (n_object * sizeof(double)));


morphological_feature_central_moments(0.0, 0.0, obj_id, width, height, n_object, image, mi00);
morphological_feature_central_moments(p, q, obj_id, width, height, n_object, image, mipq);


printf("Central invariant moment(%.0f, %.0f) :\n", p, q);
for (i = 0; i < n_object; i++){
	central_invariant_moment[i] = mipq[i] / pow(mi00[i], (1.0 + ((p+q)/2.0)));
	printf("%d %f\n", i, central_invariant_moment[i]);
	}


free(mi00);
free(mipq);
return TRUE;
}
