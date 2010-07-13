/* ############################################################################
Name           : texture_features.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : texture features

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  08/07/2010     Creation

############################################################################ */

#define __TEXTURE_FEATURES_C__
#include "texture_features.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <string.h>


/*
Grey Level Co-Occurence Matrix aka GLCM
Source: http://rsbweb.nih.gov/ij/plugins/download/GLCM_Texture.java
type: 0 0 degrees
      1 90 degrees
      2 180 degrees
      3 270 degrees

glcm[Object id][height][width]
*/
int glcm(unsigned char type, int **obj_id, int width, int height, int n_object, unsigned char *image, double ***glcm)
{
unsigned int i = 0, j = 0, k = 0;
unsigned int step = 1;
unsigned char *grey=NULL;

grey = (unsigned char *)malloc((width*height)*sizeof(unsigned char));
if (grey == NULL){
	printf("glcm:Cannot allocate %d bytes for memory.\n", ((width*height)*sizeof(unsigned char)));
	exit(-1);
	}
memset(grey, 0, (width*height)*sizeof(unsigned char));

/*Trnasform color data to greyscale*/
for (i=0;i<(height*width)*3;i+=3){
	unsigned char r = image[i+0], g = image[i+1], b = image[i+2];
	grey[(i / 3)] = GREYSCALE(r, g, b);
	}

unsigned int *pixelCounter=NULL;
pixelCounter = (unsigned int *)malloc(n_object*sizeof(unsigned int));
if (pixelCounter == NULL){
	printf("glcm:Cannot allocate %d bytes for memory.\n", (n_object*sizeof(unsigned int)));
	exit(-1);
	}
memset(pixelCounter, 0, n_object*sizeof(unsigned int));

/*
The variable a holds the value of the pixel where the Image Processor is sitting its attention.
The varialbe b holds the value of the pixel which is the neighbor to the  pixel where the 
Image Processor is sitting its attention.
*/
unsigned char a = 0, b = 0;

switch(type){
	case 0:/*0 degrees*/
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				register unsigned int ob_id = obj_id[i][j];
				if (!ob_id)
					continue;
				unsigned int idx = (j + i*width);
				unsigned int idx_step = ((j+step) + i*width);

				a = 0xff & grey[idx];
				b = 0xff & grey[idx_step];

				glcm[ob_id - 1][a][b] += 1;
				glcm[ob_id - 1][b][a] += 1;

				pixelCounter[ob_id - 1] += 2;
				}
			}


		break;
	case 1:/*90 degrees*/
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				register unsigned int ob_id = obj_id[i][j];
				if (!ob_id)
					continue;
				unsigned int idx = (j + i*width);
				unsigned int idx_step = (j + (i-step)*width);

				a = 0xff & grey[idx];
				b = 0xff & grey[idx_step];

				glcm[ob_id - 1][a][b] += 1;
				glcm[ob_id - 1][b][a] += 1;

				pixelCounter[ob_id - 1] += 2;
				}
			}

		break;
	case 2:/*180 degrees*/
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				register unsigned int ob_id = obj_id[i][j];
				if (!ob_id)
					continue;
				unsigned int idx = (j + i*width);
				unsigned int idx_step = ((j-step) + i*width);

				a = 0xff & grey[idx];
				b = 0xff & grey[idx_step];

				glcm[ob_id - 1][a][b] += 1;
				glcm[ob_id - 1][b][a] += 1;

				pixelCounter[ob_id - 1] += 2;
				}
			}

		break;
	case 3:/*270 degrees*/
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				register unsigned int ob_id = obj_id[i][j];
				if (!ob_id)
					continue;
				unsigned int idx = (j + i*width);
				unsigned int idx_step = (j + (i+step)*width);

				a = 0xff & grey[idx];
				b = 0xff & grey[idx_step];

				glcm[ob_id - 1][a][b] += 1;
				glcm[ob_id - 1][b][a] += 1;

				pixelCounter[ob_id - 1] += 2;
				}
			}

		break;
	}

/*Normalize GLCM*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcm[i][j][k] /= pixelCounter[i];
			}
		}
	}


free(pixelCounter);
free(grey);
return TRUE;
}


int texture_feature_angular_second_moment(double ***glcm, int n_object, double *angular_second_moment)
{
unsigned int i = 0, j = 0, k = 0;

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			angular_second_moment[i] += glcm[i][j][k]*glcm[i][j][k];
			}
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture angular second moment:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, angular_second_moment[i]);
	}
#endif

return TRUE;
}

int texture_feature_contrast(double ***glcm, int n_object, double *contrast)
{
unsigned int i = 0, j = 0, k = 0;

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			contrast[i] += (j-k)*(j-k)*glcm[i][j][k];
			}
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture contrast:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, contrast[i]);
	}
#endif

return TRUE;
}

int texture_feature_correlation(double ***glcm, int n_object, double *correlation)
{
unsigned int i = 0, j = 0, k = 0;
double **px;/*px = py*/
double *meanx;/*meanx = meany*/
double *stdevx;/*stdevx = stdevy*/
double *sum_sqrx;

px = (double **)malloc(n_object * sizeof(double *));
if (px == NULL){
	printf("texture_feature_correlation: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		px[i] = (double *)malloc(256 * sizeof(double));
		if (px[i] == NULL){
			printf("texture_feature_correlation: Could not allocate %d bytes for i=%d index.\n", (256 * sizeof(double)), i);
			return FALSE;
			}
		else{
			for (j = 0; j < 256; j++) {
				px[i][j] = 0.0;
				}
			}
		}/*for i*/
	}

meanx = (double *)malloc(n_object * sizeof(double));
if (meanx == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(meanx, 0, (n_object * sizeof(double)));

stdevx = (double *)malloc(n_object * sizeof(double));
if (stdevx == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(stdevx, 0, (n_object * sizeof(double)));

sum_sqrx = (double *)malloc(n_object * sizeof(double));
if (sum_sqrx == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(sum_sqrx, 0, (n_object * sizeof(double)));


for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			px[i][j] += glcm[i][j][k];
			}
		}
	}

/*Calculate meanx, sum_sqrx*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		double tmp = j * px[i][j];
		meanx[i] += tmp;
		sum_sqrx[i] += j * tmp;
		}
	}

/*Calculate standard deviation*/
for (i = 0; i < n_object; i++) {
	stdevx[i] = sqrt(sum_sqrx[i] - (meanx[i] * meanx[i]));
	}

/*Calculate correlation*/
for (i = 0; i < n_object; i++) {
	double sum = 0.0;
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			sum += glcm[i][j][k];
			}
		}
	correlation[i] = (sum - (meanx[i]*meanx[i])) / (stdevx[i]*stdevx[i]);
	}

#if ALLOW_PRINTF == TRUE
printf("Texture correlation:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, correlation[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(px[i]);
free(px);

free(meanx);
free(stdevx);
free(sum_sqrx);

return TRUE;
}


int texture_feature_variance(double ***glcm, int n_object, double *variance)
{
unsigned int i = 0, j = 0, k = 0;
double *mean;

mean = (double *)malloc(n_object * sizeof(double));
if (mean == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n_object * sizeof(double)));
	exit(-1);
	}
memset(mean, 0, (n_object * sizeof(double)));


for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			mean[i] += j * glcm[i][j][k];
			}
		}
	}

/*Calculate variance*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			double tmp = (j - mean[i]);
			variance[i] += tmp * tmp * glcm[i][j][k];
			}
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture variance:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, variance[i]);
	}
#endif

free(mean);
return TRUE;
}

int texture_feature_inverse_diff_moment(double ***glcm, int n_object, double *inverse_diff_moment)
{
unsigned int i = 0, j = 0, k = 0;

/*Calculate inverse difference moment*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			double tmp = (j - k);
			inverse_diff_moment[i] += glcm[i][j][k] / (1.0 + (tmp*tmp)) ;
			}
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture inverse difference moment:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, inverse_diff_moment[i]);
	}
#endif

return TRUE;
}

int texture_feature_sum_average(double ***glcm, int n_object, double *sum_average)
{
unsigned int i = 0, j = 0, k = 0;
double **glcmxpy;

glcmxpy = (double **)malloc(n_object * sizeof(double *));
if (glcmxpy == NULL){
	printf("texture_feature_correlation: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		glcmxpy[i] = (double *)malloc((2 * 256) * sizeof(double));
		if (glcmxpy[i] == NULL){
			printf("Cannot allocate %d bytes for memory.\n", ((2 * 256) * sizeof(double)));
			exit(-1);
			}
		else{
			for (j = 0; j < (2 * 256); j++)
				glcmxpy[i][j] = 0.0;
			}
		}
	}

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcmxpy[i][j+k] += glcm[i][j][k];
			}
		}
	}

/*Calculate sum average*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j <= (2*256 - 2); j++) {
			sum_average[i] += j * glcmxpy[i][j];
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture sum average:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, sum_average[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(glcmxpy[i]);
free(glcmxpy);

return TRUE;
}

int texture_feature_sum_entropy(double ***glcm, int n_object, double *sum_entropy)
{
unsigned int i = 0, j = 0, k = 0;
double **glcmxpy;

glcmxpy = (double **)malloc(n_object * sizeof(double *));
if (glcmxpy == NULL){
	printf("texture_feature_sum_entropy: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		glcmxpy[i] = (double *)malloc((2 * 256) * sizeof(double));
		if (glcmxpy[i] == NULL){
			printf("Cannot allocate %d bytes for memory.\n", ((2 * 256) * sizeof(double)));
			exit(-1);
			}
		else{
			for (j = 0; j < (2 * 256); j++)
				glcmxpy[i][j] = 0.0;
			}
		}
	}

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcmxpy[i][j+k] += glcm[i][j][k];
			}
		}
	}

/*Calculate sum entropy*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j <= (2*256 - 2); j++) {
			sum_entropy[i] -= glcmxpy[i][j] * log10(glcmxpy[i][j] + SMALL_NUMBER);
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture sum entropy:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, sum_entropy[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(glcmxpy[i]);
free(glcmxpy);

return TRUE;
}

int texture_feature_sum_variance(double ***glcm, int n_object, double *sum_entropy, double *sum_variance)
{
unsigned int i = 0, j = 0, k = 0;
double **glcmxpy;

glcmxpy = (double **)malloc(n_object * sizeof(double *));
if (glcmxpy == NULL){
	printf("texture_feature_sum_entropy: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		glcmxpy[i] = (double *)malloc((2 * 256) * sizeof(double));
		if (glcmxpy[i] == NULL){
			printf("Cannot allocate %d bytes for memory.\n", ((2 * 256) * sizeof(double)));
			exit(-1);
			}
		else{
			for (j = 0; j < (2 * 256); j++)
				glcmxpy[i][j] = 0.0;
			}
		}
	}

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcmxpy[i][j+k] += glcm[i][j][k];
			}
		}
	}

/*Calculate sum variance*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j <= (2*256 - 2); j++) {
			double tmp = (j - sum_entropy[i]);
			sum_variance[i] += tmp * tmp * glcmxpy[i][j];
		}
	}

#if ALLOW_PRINTF == TRUE
printf("Texture sum variance:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, sum_variance[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(glcmxpy[i]);
free(glcmxpy);

return TRUE;
}

int texture_feature_entropy(double ***glcm, int n_object, double *entropy)
{
unsigned int i = 0, j = 0, k = 0;

/*Calculate entropy*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			entropy[i] += glcm[i][j][k] * log10(glcm[i][j][k] + SMALL_NUMBER);
			}
		}
	entropy[i] = -1.0 * entropy[i];
	}

#if ALLOW_PRINTF == TRUE
printf("Texture entropy:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, entropy[i]);
	}
#endif

return TRUE;
}

int texture_feature_difference_variance(double ***glcm, int n_object, double *difference_variance)
{
unsigned int i = 0, j = 0, k = 0;
double **glcmxmy;

glcmxmy = (double **)malloc(n_object * sizeof(double *));
if (glcmxmy == NULL){
	printf("texture_feature_sum_entropy: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		glcmxmy[i] = (double *)malloc(256 * sizeof(double));
		if (glcmxmy[i] == NULL){
			printf("Cannot allocate %d bytes for memory.\n", (256 * sizeof(double)));
			exit(-1);
			}
		else{
			for (j = 0; j < 256; j++)
				glcmxmy[i][j] = 0.0;
			}
		}
	}

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcmxmy[i][abs(j-k)] += glcm[i][j][k];
			}
		}
	}


/*Calculate difference variance*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
			difference_variance[i] +=  j * j * glcmxmy[i][j];
		}
	}


#if ALLOW_PRINTF == TRUE
printf("Texture difference variance:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, difference_variance[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(glcmxmy[i]);
free(glcmxmy);

return TRUE;
}

int texture_feature_difference_entropy(double ***glcm, int n_object, double *difference_entropy)
{
unsigned int i = 0, j = 0, k = 0;
double **glcmxmy;

glcmxmy = (double **)malloc(n_object * sizeof(double *));
if (glcmxmy == NULL){
	printf("texture_feature_sum_entropy: Could not allocate %d bytes.\n", (n_object * sizeof(double *)));
	return FALSE;
	}
else{
	for (i=0;i<n_object;i++){
		glcmxmy[i] = (double *)malloc(256 * sizeof(double));
		if (glcmxmy[i] == NULL){
			printf("Cannot allocate %d bytes for memory.\n", (256 * sizeof(double)));
			exit(-1);
			}
		else{
			for (j = 0; j < 256; j++)
				glcmxmy[i][j] = 0.0;
			}
		}
	}

for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcmxmy[i][abs(j-k)] += glcm[i][j][k];
			}
		}
	}


/*Calculate difference entropy*/
for (i = 0; i < n_object; i++) {
	for (j = 0; j < 256; j++) {
			difference_entropy[i] +=  glcmxmy[i][j] * log10(glcmxmy[i][j] + SMALL_NUMBER);
		}
	difference_entropy[i] = -1.0 * difference_entropy[i];
	}

#if ALLOW_PRINTF == TRUE
printf("Texture difference entropy:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, difference_entropy[i]);
	}
#endif

for(i=0;i<n_object;i++)
	free(glcmxmy[i]);
free(glcmxmy);

return TRUE;
}
