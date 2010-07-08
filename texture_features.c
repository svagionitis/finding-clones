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
unsigned char *grey;

grey = (unsigned char *)malloc((width*height)*sizeof(unsigned char));
if (grey == NULL){
	printf("glcm:Cannot allocate %d bytes for memory.\n", ((width*height)*sizeof(unsigned char)));
	exit(-1);
	}
memset(grey, 0, sizeof((width*height)*sizeof(unsigned char)));

/*Trnasform color data to greyscale*/
for (i=0;i<(height*width)*3;i+=3){
	grey[i] = GREYSCALE(image[i+0], image[i+1], image[i+2]);
	}

unsigned int *pixelCounter;
pixelCounter = (unsigned int *)malloc(n_object*sizeof(unsigned int));
if (pixelCounter == NULL){
	printf("glcm:Cannot allocate %d bytes for memory.\n", (n_object*sizeof(unsigned int)));
	exit(-1);
	}
memset(pixelCounter, 0, sizeof(n_object*sizeof(unsigned int)));

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
	printf("ID: %d\n", i);
	for (j = 0; j < 257; j++) {
		for (k = 0;k < 257;k++){
			glcm[i][j][k] /= pixelCounter[i];
			printf("%.3f ", glcm[i][j][k]);
			}
		printf("\n");
		}
	printf("\n");
	}


free(pixelCounter);
free(grey);
return TRUE;
}


