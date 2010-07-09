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
	/*printf("ID: %d\n", i);*/
	for (j = 0; j < 256; j++) {
		for (k = 0;k < 256;k++){
			glcm[i][j][k] /= pixelCounter[i];
			/*printf("%f ", glcm[i][j][k]);*/
			}
		/*printf("\n");*/
		}
	/*printf("\n");*/
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

printf("Texture angular second moment:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, angular_second_moment[i]);
	}

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

printf("Texture contrast:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, contrast[i]);
	}

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


printf("Texture correlation:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, correlation[i]);
	}

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


printf("Texture variance:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, variance[i]);
	}

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

printf("Texture inverse difference moment:\n");
for (i = 0; i < n_object; i++) {
	printf("%d %.3f\n", i, inverse_diff_moment[i]);
	}

return TRUE;
}

