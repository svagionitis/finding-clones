/* ############################################################################
Name           : color_features.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Color features

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  07/07/2010     Creation

############################################################################ */

#define __COLOR_FEATURES_C__
#include "color_features.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <string.h>


/*First color moment: mean*/
int color_feature_mean(int **obj_id, int width, int height, int n_object, int *area, unsigned char *image, double *color_mean_value)
{
unsigned int i = 0, j = 0;

unsigned int *sum = NULL;
sum = (unsigned int *)malloc(n_object * sizeof(unsigned int));
if (sum == NULL){
	printf("color_feature_mean:Cannot allocate %d bytes for memory.\n", (n_object * sizeof(unsigned int)));
	exit(-1);
	}
memset(sum, 0, sizeof(n_object * sizeof(unsigned int)));


for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
		if (!obj_id[i][j])
			continue;

		register unsigned int idx = (j + i*width)*3;

		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);

		sum[obj_id[i][j]-1] += (unsigned int)val;
		}
	}

printf("Color mean value:\n");
for (i = 0; i < n_object; i++) {
	color_mean_value[i] = ((double)sum[i] / area[i]);
	printf("%d %.3f\n", i, color_mean_value[i]);
	}


free(sum);
return TRUE;
}

/*Second color moment: standard deviation->describes how similar the intensities are within the region.*/
int color_feature_standard_deviation(int **obj_id, int width, int height, int n_object, int *area, unsigned char *image, double *mean, double *color_std_dev)
{
unsigned int i = 0, j = 0;

unsigned int *sum = NULL;
sum = (unsigned int *)malloc(n_object * sizeof(unsigned int));
if (sum == NULL){
	printf("color_feature_standard_deviation:Cannot allocate %d bytes for memory.\n", (n_object * sizeof(unsigned int)));
	exit(-1);
	}
memset(sum, 0, sizeof(n_object * sizeof(unsigned int)));


for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
		if (!obj_id[i][j])
			continue;

		register unsigned int idx = (j + i*width)*3;

		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);
		double tmp = ((double)val - mean[obj_id[i][j]-1]);
		sum[obj_id[i][j]-1] += (unsigned int)(tmp*tmp);
		}
	}

printf("Color standard deviation:\n");
for (i = 0; i < n_object; i++) {
	color_std_dev[i] = sqrt((double)sum[i] / area[i]);
	printf("%d %.3f\n", i, color_std_dev[i]);
	}


free(sum);
return TRUE;
}

/*Third color moment: skewness->describes how symmetric the intensity distribution is about the mean.*/
int color_feature_skewness(int **obj_id, int width, int height, int n_object, int *area, unsigned char *image, double *mean, double *color_skew)
{
unsigned int i = 0, j = 0;

unsigned int *sum = NULL;
sum = (unsigned int *)malloc(n_object * sizeof(unsigned int));
if (sum == NULL){
	printf("color_feature_skewness:Cannot allocate %d bytes for memory.\n", (n_object * sizeof(unsigned int)));
	exit(-1);
	}
memset(sum, 0, sizeof(n_object * sizeof(unsigned int)));


for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
		if (!obj_id[i][j])
			continue;

		register unsigned int idx = (j + i*width)*3;

		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);
		double tmp = ((double)val - mean[obj_id[i][j]-1]);
		sum[obj_id[i][j]-1] += (unsigned int)(tmp*tmp*tmp);
		}
	}

printf("Color skewness:\n");
for (i = 0; i < n_object; i++) {
	color_skew[i] = pow(((double)sum[i] / area[i]), 1.0/3.0);
	printf("%d %.3f\n", i, color_skew[i]);
	}


free(sum);
return TRUE;
}

/*Forth color moment: kurtosis ->describes how ﬂat the distribution is.*/
int color_feature_kurtosis(int **obj_id, int width, int height, int n_object, int *area, unsigned char *image, double *mean, double *color_kurtosis)
{
unsigned int i = 0, j = 0;

unsigned int *sum = NULL;
sum = (unsigned int *)malloc(n_object * sizeof(unsigned int));
if (sum == NULL){
	printf("color_feature_kurtosis:Cannot allocate %d bytes for memory.\n", (n_object * sizeof(unsigned int)));
	exit(-1);
	}
memset(sum, 0, sizeof(n_object * sizeof(unsigned int)));


for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
		if (!obj_id[i][j])
			continue;

		register unsigned int idx = (j + i*width)*3;

		unsigned char val = GREYSCALE(image[idx], image[idx+1], image[idx+2]);
		double tmp = ((double)val - mean[obj_id[i][j]-1]);
		sum[obj_id[i][j]-1] += (unsigned int)(tmp*tmp*tmp*tmp);
		}
	}

printf("Color kurtosis:\n");
for (i = 0; i < n_object; i++) {
	color_kurtosis[i] = pow(((double)sum[i] / area[i]), 1.0/4.0);
	printf("%d %.3f\n", i, color_kurtosis[i]);
	}


free(sum);
return TRUE;
}
