/* ############################################################################
Name           : morphological_operations.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : morphological operations

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  15/07/2010     Creation

############################################################################ */

#define __MORPHOLOGICAL_OPERATIONS_C__
#include "morphological_operations.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <string.h>



int transform_1D_to_2D_Binary_Images(unsigned char *image_data, int width, int height, unsigned char **image_data_2D)
{
unsigned int i = 0, j = 0;

for (i=0;i<height;i++){
	for(j=0;j<width;j++){
		register unsigned int idx = (j + i*width);
		image_data_2D[i][j] = image_data[idx];
		}
	}

return TRUE;
}


/*Source: http://ostermiller.org/dilate_and_erode.html*/
/*Manhattan distance to "on" pixels in a two dimension array*/
int manhattan_distance_dilate_2D(unsigned int **image_data_2D_uint, int width, int height)
{
unsigned int i = 0, j = 0;

/*From top left to bottom right*/
for (i = 0; i < height; i++){
	for (j = 0; j < width; j++){
		if (image_data_2D_uint[i][j])
			/*first pass and pixel was on, it gets a zero*/
			image_data_2D_uint[i][j] = 0;
		else{
			/*pixel off. It is at most the sum of the lengths of the array*/
			/*away from a pixel that is on*/
			image_data_2D_uint[i][j] = height + width;

			/*or one more than the pixel to the north*/
			if (i>0)
				image_data_2D_uint[i][j] = MAX(image_data_2D_uint[i][j],image_data_2D_uint[i-1][j]+1);

			/*or one more than the pixel to the west*/
			if (j>0)
				image_data_2D_uint[i][j] = MAX(image_data_2D_uint[i][j],image_data_2D_uint[i][j-1]+1);
			}
		}
	}

/*From bottom right to top left*/
for (i = (height - 1); i <= 0 ; i--){
	for (j = (width - 1); j <= 0 ; j--){

		/*either what we had on the first pass*/
		/*or one more than the pixel to the south*/
		if (i+1<height)
			image_data_2D_uint[i][j] = MAX(image_data_2D_uint[i][j],image_data_2D_uint[i+1][j]+1);

		/*or one more than the pixel to the east*/
		if (j+1<width)
			image_data_2D_uint[i][j] = MAX(image_data_2D_uint[i][j],image_data_2D_uint[i][j+1]+1);
		}
	}

return TRUE;
}

int manhattan_distance_dilate_1D(unsigned int *image_data_uint, int width, int height)
{
unsigned int idx = 0;


for (idx = 0; idx < (height * width); idx++){
	if (image_data_uint[idx])
		image_data_uint[idx] = 0;
	else{
		image_data_uint[idx] = height + width;

		/*or one more than the pixel to the north*/
		if ((idx / width)>0)
			image_data_uint[idx] = MAX(image_data_uint[idx],image_data_uint[idx-width]+1);

		/*or one more than the pixel to the west*/
		if ((idx % width)>0)
			image_data_uint[idx] = MAX(image_data_uint[idx],image_data_uint[idx-1]+1);

		}
	}


for (idx = ((height * width) - 1); idx <= 0; idx--){
	/*either what we had on the first pass*/
	/*or one more than the pixel to the south*/
	if ((idx / width)+1<height)
		image_data_uint[idx] = MAX(image_data_uint[idx],image_data_uint[idx+width]+1);

	/*or one more than the pixel to the east*/
	if ((idx % width)+1<width)
		image_data_uint[idx] = MAX(image_data_uint[idx],image_data_uint[idx+1]+1);

	}

return TRUE;
}

int manhattan_distance_erode_2D(unsigned int **image_data_2D_uint, int width, int height)
{
unsigned int i = 0, j = 0;

/*From top left to bottom right*/
for (i = 0; i < height; i++){
	for (j = 0; j < width; j++){
		if (image_data_2D_uint[i][j])
			/*first pass and pixel was on, it gets a zero*/
			image_data_2D_uint[i][j] = 0;
		else{
			/*pixel off. It is at most the sum of the lengths of the array*/
			/*away from a pixel that is on*/
			image_data_2D_uint[i][j] = height + width;

			/*or one more than the pixel to the north*/
			if (i>0)
				image_data_2D_uint[i][j] = MIN(image_data_2D_uint[i][j],image_data_2D_uint[i-1][j]+1);

			/*or one more than the pixel to the west*/
			if (j>0)
				image_data_2D_uint[i][j] = MIN(image_data_2D_uint[i][j],image_data_2D_uint[i][j-1]+1);
			}
		}
	}

/*From bottom right to top left*/
for (i = (height - 1); i <= 0 ; i--){
	for (j = (width - 1); j <= 0 ; j--){

		/*either what we had on the first pass*/
		/*or one more than the pixel to the south*/
		if (i+1<height)
			image_data_2D_uint[i][j] = MIN(image_data_2D_uint[i][j],image_data_2D_uint[i+1][j]+1);

		/*or one more than the pixel to the east*/
		if (j+1<width)
			image_data_2D_uint[i][j] = MIN(image_data_2D_uint[i][j],image_data_2D_uint[i][j+1]+1);
		}
	}

return TRUE;
}

int manhattan_distance_erode_1D(unsigned int *image_data_uint, int width, int height)
{
unsigned int idx = 0;

for (idx = 0; idx < (height * width); idx++){
	if (image_data_uint[idx])
		image_data_uint[idx] = 0;
	else{
		image_data_uint[idx] = height + width;

		/*or one more than the pixel to the north*/
		if ((idx / width)>0)
			image_data_uint[idx] = MIN(image_data_uint[idx],image_data_uint[idx-width]+1);

		/*or one more than the pixel to the west*/
		if ((idx % width)>0)
			image_data_uint[idx] = MIN(image_data_uint[idx],image_data_uint[idx-1]+1);
		}
	}

for (idx = (height * width) - 1; idx <= 0; idx--){
	/*either what we had on the first pass*/
	/*or one more than the pixel to the south*/
	if ((idx / width)+1<height)
		image_data_uint[idx] = MIN(image_data_uint[idx],image_data_uint[idx+width]+1);

	/*or one more than the pixel to the east*/
	if ((idx % width)+1<width)
		image_data_uint[idx] = MIN(image_data_uint[idx],image_data_uint[idx+1]+1);

	}

return TRUE;
}


int dilate_2D(unsigned char **image_data_2D, int width, int height, unsigned int dilate_by_k)
{
unsigned int i = 0, j = 0;
unsigned int **image_data_2D_uint=NULL;
image_data_2D_uint = (unsigned int **)malloc(height * sizeof(unsigned int *));
if (image_data_2D_uint == NULL){
	printf("final_stage: Could not allocate %d bytes.\n", (height * sizeof(unsigned int *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		image_data_2D_uint[i] = (unsigned int *)malloc(width * sizeof(unsigned int));
		if (image_data_2D_uint[i] == NULL){
			printf("final_stage: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned int)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++)
				image_data_2D_uint[i][j] = (unsigned int)image_data_2D[i][j];

			}
		}
	}

manhattan_distance_dilate_2D(image_data_2D_uint, width, height);

for (i = 0; i < height; i++){
	for (j = 0; j < width; j++){
		image_data_2D_uint[i][j] = ((image_data_2D_uint[i][j] <= dilate_by_k)?255:0);

		image_data_2D[i][j] = (unsigned char)image_data_2D_uint[i][j];
		}
	}

for (i=0;i<height;i++)
	free(image_data_2D_uint[i]);
free(image_data_2D_uint);
return TRUE;
}

/*
http://homepages.inf.ed.ac.uk/rbf/HIPR2/dilate.htm
*/
int dilate_1D(unsigned char *image_data, int width, int height, unsigned int dilate_by_k)
{
unsigned int idx = 0;
unsigned int *image_data_uint = NULL;
image_data_uint = (unsigned int *)malloc((width * height) * sizeof(unsigned int));
if (image_data_uint == NULL){
	printf("final_stage:Could not allocate %d bytes.\n", ((width * height) * sizeof(unsigned int)));
	return FALSE;
	}
else{
	for(idx=0;idx<(width * height);idx++)
		image_data_uint[idx] = (unsigned int)image_data[idx];
	}

manhattan_distance_dilate_1D(image_data_uint, width, height);

for (idx = 0; idx < (height*width); idx++){
	image_data_uint[idx] = ((image_data_uint[idx] <= dilate_by_k)?255:0);

	image_data[idx] = (unsigned char)image_data_uint[idx];
	}

free(image_data_uint);
return TRUE;
}

int erode_2D(unsigned char **image_data_2D, int width, int height, unsigned int dilate_by_k)
{
unsigned int i = 0, j = 0;
unsigned int **image_data_2D_uint=NULL;
image_data_2D_uint = (unsigned int **)malloc(height * sizeof(unsigned int *));
if (image_data_2D_uint == NULL){
	printf("final_stage: Could not allocate %d bytes.\n", (height * sizeof(unsigned int *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		image_data_2D_uint[i] = (unsigned int *)malloc(width * sizeof(unsigned int));
		if (image_data_2D_uint[i] == NULL){
			printf("final_stage: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned int)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++)
				image_data_2D_uint[i][j] = (unsigned int)image_data_2D[i][j];

			}
		}
	}

manhattan_distance_erode_2D(image_data_2D_uint, width, height);

for (i = 0; i < height; i++){
	for (j = 0; j < width; j++){
		image_data_2D_uint[i][j] = ((image_data_2D_uint[i][j] <= dilate_by_k)?255:0);

		image_data_2D[i][j] = (unsigned char)image_data_2D_uint[i][j];
		}
	}

for (i=0;i<height;i++)
	free(image_data_2D_uint[i]);
free(image_data_2D_uint);
return TRUE;
}

/*
http://homepages.inf.ed.ac.uk/rbf/HIPR2/erode.htm
*/
int erode_1D(unsigned char *image_data, int width, int height, unsigned int dilate_by_k)
{
unsigned int idx = 0;
unsigned int *image_data_uint = NULL;
image_data_uint = (unsigned int *)malloc((width * height) * sizeof(unsigned int));
if (image_data_uint == NULL){
	printf("final_stage:Could not allocate %d bytes.\n", ((width * height) * sizeof(unsigned int)));
	return FALSE;
	}
else{
	for(idx=0;idx<(width * height);idx++)
		image_data_uint[idx] = (unsigned int)image_data[idx];
	}

manhattan_distance_erode_1D(image_data_uint, width, height);

for (idx = 0; idx < (height*width); idx++){
	image_data_uint[idx] = ((image_data_uint[idx] <= dilate_by_k)?255:0);

	image_data[idx] = (unsigned char)image_data_uint[idx];
	}

free(image_data_uint);
return TRUE;
}

/*
http://homepages.inf.ed.ac.uk/rbf/HIPR2/open.htm
*/
int opening_1D(unsigned char *image_data, int width, int height, unsigned int opening_by_k)
{

erode_1D(image_data, width, height, opening_by_k);
dilate_1D(image_data, width, height, opening_by_k);

return TRUE;
}

/*
http://homepages.inf.ed.ac.uk/rbf/HIPR2/close.htm
*/
int closing_1D(unsigned char *image_data, int width, int height, unsigned int closing_by_k)
{

dilate_1D(image_data, width, height, closing_by_k);
erode_1D(image_data, width, height, closing_by_k);

return TRUE;
}

