/* ############################################################################
Name           : alg2.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm 2. Implementing some edge detection 
                 algorithms.

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */

#define __ALG2_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "alg2.h"
#include "alcon2009.h"/*For save_ppm function*/

unsigned char ***data2D;
sobel **sobel_data;

/* ############################################################################
Name           : transform_1D_to_2D_RGB
Description    : Transform 1D data to 2D for better handling and maybe faster.

Arguments             Type                Description
===================== =================== =====================================
image_data(IN)        unsigned char *     Image data.
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================
data2D(OUT)           unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int transform_1D_to_2D(unsigned char *image_data, int width, int height)
{
unsigned int i = 0, j = 0;

/* Allocate memory for subimage data*/
data2D = (unsigned char ***)malloc(height * sizeof(unsigned char **));
if (data2D == NULL){
	printf("transform_1D_to_2D: Could not allocate %d bytes.\n", (height * sizeof(unsigned char **)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data2D[i] = (unsigned char **)malloc(width * sizeof(unsigned char*));
		if (data2D[i] == NULL){
			printf("transform_1D_to_2D: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char*)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data2D[i][j] = (unsigned char *)malloc(3 * sizeof(unsigned char));
				if (data2D[i][j] == NULL){
					printf("transform_1D_to_2D: Could not allocate %d bytes for j=%d index.\n", (3 * sizeof(unsigned char)), j);
					return FALSE;
					}
				else{
					data2D[i][j][0] = 0;
					data2D[i][j][1] = 0;
					data2D[i][j][2] = 0;
					}
				}/*for j*/
			}
		}/*for i*/
	printf("transform_1D_to_2D: Allocated %d bytes.\n", (3 * width * height * sizeof(unsigned char)));
	}


for (i=0;i<height;i++){
	for(j=0;j<width;j++){
		register unsigned int idx = (j + i*width)*3;
		data2D[i][j][0] = image_data[idx + 0];
		data2D[i][j][1] = image_data[idx + 1];
		data2D[i][j][2] = image_data[idx + 2];
		}
	}

return TRUE;
}


/* ############################################################################
Name           : export_ppm_from_2D
Description    : Create ppm images according to type.

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       0 for red value pixels
                                          1 for green value pixels
                                          2 for blue value pixels
                                          3 for grey value pixels
                                          4 for RGB value pixels
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================
data2D(IN)            unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.

############################################################################ */
int export_ppm_from_2D(unsigned char type, int width, int height)
{
unsigned int i = 0, j = 0;
char filename[128];
memset(filename, '\0', sizeof(filename));

unsigned char *data_buffer=NULL;
unsigned int data_mem_alloc = (3 * width * height);
data_buffer = (unsigned char *)malloc(data_mem_alloc * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("export_ppm_from_2D: Could not allocate %d bytes.\n", (data_mem_alloc * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	printf("export_ppm_from_2D: Mem alloc %d bytes.\n", data_mem_alloc * sizeof(unsigned char));
	for(i=0;i<data_mem_alloc;i++)
		data_buffer[i] = 0;
	}

switch(type){
	case 0:/*Red values*/
		sprintf(filename,"RedImage.ppm");
		break;
	case 1:/*Green values*/
		sprintf(filename,"GreenImage.ppm");
		break;
	case 2:/*Blue values*/
		sprintf(filename,"BlueImage.ppm");
		break;
	case 3:/*Grey values*/
		sprintf(filename,"GreyImage.ppm");
		break;
	case 4:/*RGB values*/
		sprintf(filename,"RGBImage.ppm");
		break;
	}

for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		register unsigned int idx = (j + i*width)*3;
		switch(type){
			unsigned char grey = 0;
			case 0:/*Red values*/
				data_buffer[idx + 0] = data2D[i][j][0];
				data_buffer[idx + 1] = 0;
				data_buffer[idx + 2] = 0;
				break;
			case 1:/*Green values*/
				data_buffer[idx + 0] = 0;
				data_buffer[idx + 1] = data2D[i][j][1];
				data_buffer[idx + 2] = 0;
				break;
			case 2:/*Blue values*/
				data_buffer[idx + 0] = 0;
				data_buffer[idx + 1] = 0;
				data_buffer[idx + 2] = data2D[i][j][2];
				break;
			case 3:/*Grey values*/
				grey = GREYSCALE1(data2D[i][j][0], data2D[i][j][1], data2D[i][j][2]);
				data_buffer[idx + 0] = grey;
				data_buffer[idx + 1] = grey;
				data_buffer[idx + 2] = grey;
				break;
			case 4:/*RGB values*/
				data_buffer[idx + 0] = data2D[i][j][0];
				data_buffer[idx + 1] = data2D[i][j][1];
				data_buffer[idx + 2] = data2D[i][j][2];
				break;
			}
		
		}
	}


save_ppm(filename, width, height, data_buffer);


free(data_buffer);


return TRUE;
}

/* ############################################################################
Name           : convert_to_greyscale
Description    : Convert color RGB to greyscale image

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
data2D(OUT)           unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int convert_to_greyscale(int width, int height)
{
int i = 0, j = 0;

/*Convert to greyscale*/
for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		unsigned char grey = 0;
		grey = GREYSCALE1(data2D[i][j][0], data2D[i][j][1], data2D[i][j][2]);
		data2D[i][j][0] = grey;
		data2D[i][j][1] = grey;
		data2D[i][j][2] = grey;
		}
	}

return TRUE;
}

/* ############################################################################
Name           : convert_to_red
Description    : Convert color RGB to red values image.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
data2D(OUT)           unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int convert_to_red(int width, int height)
{
int i = 0, j = 0;

/*Convert to red values*/
for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][1] = 0;
		data2D[i][j][2] = 0;
		}
	}

return TRUE;
}

/* ############################################################################
Name           : convert_to_green
Description    : Convert color RGB to green values image.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
data2D(OUT)           unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int convert_to_green(int width, int height)
{
int i = 0, j = 0;

/*Convert to green values*/
for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = 0;
		data2D[i][j][2] = 0;
		}
	}

return TRUE;
}

/* ############################################################################
Name           : convert_to_blue
Description    : Convert color RGB to blue values image.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
data2D(OUT)           unsigned char ***   Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int convert_to_blue(int width, int height)
{
int i = 0, j = 0;

/*Convert to blue values*/
for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = 0;
		data2D[i][j][1] = 0;
		}
	}

return TRUE;
}

int noise_reduction(int width, int height)
{
int i = 0, j = 0;
/*
float a = 2.0, b = 4.0, c = 5.0;
float d = 9.0, e = 12.0, f = 15.0;
float inv = 1.0/159.0;
*/
float a = 1.0, b = 4.0, c = 7.0;
float d = 16.0, e = 26.0, f = 41.0;
float inv = 1.0/273.0;

float Gaussian_Filter[5][5] = {{a, b, c, b, a},
			       {b, d, e, d, b},
			       {c, e, f, e, c},
			       {b, d, e, d, b},
			       {a, b, c, b, a}};

/* Allocate temporary memory to store the filtered data from greyscale image.*/
unsigned char **data_buffer=NULL;
data_buffer = (unsigned char **)malloc(height * sizeof(unsigned char *));
if (data_buffer == NULL){
	printf("noise_reduction: Could not allocate %d bytes.\n", (height * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data_buffer[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (data_buffer[i] == NULL){
			printf("noise_reduction: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data_buffer[i][j] = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("noise_reduction: Allocated %d bytes.\n", (width * height * sizeof(unsigned char)));
	}


for(i=0;i<height;i++){
	int im2 = (i-2), im1 = (i-1), ip2 = (i+2), ip1 = (i+1);
	for(j=0;j<width;j++){
		int jm2 = (j-2), jm1 = (j-1), jp2 = (j+2), jp1 = (j+1);
		float filter_out = 0.0;

		if (((im2<0) && (im1<0) && (jm2<0) && (jm1<0)) &&
                    ((ip2>=0) && (ip1>=0) && (jp2>=0) && (jp1>=0))){/*Top-Left corner*/
		filter_out =  ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
			      ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2<0) && (im1<0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1>=0) && (jp2>=0) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2<0) && (im1<0) && (jm2>=0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1>=0) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2<0) && (im1>=0) && (jm2>=0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];

			}
		else if(((im2<0) && (im1>=0) && (jm2<0) && (jm1<0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		filter_out =  ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2>=0) && (im1>=0) && (jm2<0) && (jm1<0)) &&
                        ((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		filter_out =  ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2>=0) && (im1>=0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>=0) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
			      ((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}
		else if(((im2<0) && (im1>=0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2>=0) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
			      ((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];
			}/********************************************************************************************************************/
		else if (((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){/*Top-Right corner*/
		filter_out =  ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+
			      ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2];
			}
		else if (((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+
			      ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3];
			}
		else if (((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+
			      ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2];

			}
		else if (((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+
			      ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+
			      ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2];
			}
		else if (((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3];

			}
		else if (((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3];
			}/********************************************************************************************************************/
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1<0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){/*Bottom-Left corner*/
		filter_out =  ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4];
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		filter_out =  ((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4];
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2>=0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4];
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2>=0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4];
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1<0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		filter_out =  ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4];
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		filter_out =  ((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
			      ((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
			      ((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
			      ((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4];
			}/********************************************************************************************************************/
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){/*Bottom-Right corner*/
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+
			      ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2];
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3];
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+
			      ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+
			      ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+
			      ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2];
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2>=0) && (jm1>=0))){
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+ 
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3];
			}/********************************************************************************************************************/
		else{
		filter_out =  ((float)data2D[i-2][j-2][0])*Gaussian_Filter[0][0]+((float)data2D[i-2][j-1][0])*Gaussian_Filter[0][1]+
                              ((float)data2D[i-2][j  ][0])*Gaussian_Filter[0][2]+((float)data2D[i-2][j+1][0])*Gaussian_Filter[0][3]+
                              ((float)data2D[i-2][j+2][0])*Gaussian_Filter[0][4]+
                              ((float)data2D[i-1][j-2][0])*Gaussian_Filter[1][0]+((float)data2D[i-1][j-1][0])*Gaussian_Filter[1][1]+
                              ((float)data2D[i-1][j  ][0])*Gaussian_Filter[1][2]+((float)data2D[i-1][j+1][0])*Gaussian_Filter[1][3]+ 
                              ((float)data2D[i-1][j+2][0])*Gaussian_Filter[1][4]+
                              ((float)data2D[i  ][j-2][0])*Gaussian_Filter[2][0]+((float)data2D[i  ][j-1][0])*Gaussian_Filter[2][1]+
                              ((float)data2D[i  ][j  ][0])*Gaussian_Filter[2][2]+((float)data2D[i  ][j+1][0])*Gaussian_Filter[2][3]+ 
                              ((float)data2D[i  ][j+2][0])*Gaussian_Filter[2][4]+
                              ((float)data2D[i+1][j-2][0])*Gaussian_Filter[3][0]+((float)data2D[i+1][j-1][0])*Gaussian_Filter[3][1]+
                              ((float)data2D[i+1][j  ][0])*Gaussian_Filter[3][2]+((float)data2D[i+1][j+1][0])*Gaussian_Filter[3][3]+ 
                              ((float)data2D[i+1][j+2][0])*Gaussian_Filter[3][4]+
                              ((float)data2D[i+2][j-2][0])*Gaussian_Filter[4][0]+((float)data2D[i+2][j-1][0])*Gaussian_Filter[4][1]+
                              ((float)data2D[i+2][j  ][0])*Gaussian_Filter[4][2]+((float)data2D[i+2][j+1][0])*Gaussian_Filter[4][3]+ 
                              ((float)data2D[i+2][j+2][0])*Gaussian_Filter[4][4];

			}

		data_buffer[i][j] = ((unsigned char)(filter_out*inv));
		}
	}


for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = data_buffer[i][j];
		data2D[i][j][1] = data_buffer[i][j];
		data2D[i][j][2] = data_buffer[i][j];
		}
	}


for(i=0;i<height;i++)
	free(data_buffer[i]);
free(data_buffer);

return TRUE;
}

/*
type: 0 magnitude with square root
      1 magnitude with absolute value
      2 angle, direction
      3 Calculate magnitude using sqrt and direction
*/
int Sobel_operator(unsigned char type, int width, int height)
{
int i = 0, j = 0;

float Gx[3][3] = {{-1.0, 0.0, 1.0}, 
		  {-2.0, 0.0, 2.0}, 
		  {-1.0, 0.0, 1.0}};

float Gy[3][3] = {{ 1.0,  2.0,  1.0}, 
		  { 0.0,  0.0,  0.0}, 
		  {-1.0, -2.0, -1.0}};

/* Allocate memory for magnitude and direction of sobel operator.*/
sobel_data = (sobel **)malloc(height * sizeof(sobel *));
if (sobel_data == NULL){
	printf("Sobel_operator: Could not allocate %d bytes.\n", (height * sizeof(sobel *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		sobel_data[i] = (sobel *)malloc(width * sizeof(sobel));
		if (sobel_data[i] == NULL){
			printf("Sobel_operator: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(sobel)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				sobel_data[i][j].magnitude = 0;
				sobel_data[i][j].direction = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("Sobel_operator: Allocated %d bytes.\n", (width * height * sizeof(sobel)));
	}

/* Allocate temporary memory to store the values after sobel operator has passed.*/
unsigned char **data_buffer=NULL;
data_buffer = (unsigned char **)malloc(height * sizeof(unsigned char *));
if (data_buffer == NULL){
	printf("Sobel_operator: Could not allocate %d bytes.\n", (height * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data_buffer[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (data_buffer[i] == NULL){
			printf("Sobel_operator: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data_buffer[i][j] = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("Sobel_operator: Allocated %d bytes.\n", (width * height * sizeof(unsigned char)));
	}

for(i=0;i<height;i++){
	int im1 = (i-1), ip1 = (i+1);
	for(j=0;j<width;j++){
		int jm1 = (j-1), jp1 = (j+1);
		float sobelx_out = 0.0, sobely_out = 0.0;
		float magnitude_ttl = 0.0;
		float angle = 0.0, theta = 0.0;

		if (((im1<0) && (jm1<0)) &&
                    ((ip1>=0) &&(jp1>=0))){/*Top-Left corner*/
		sobelx_out =((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}
		else if(((im1<0) && (jm1>=0)) &&
                        ((ip1>=0) && (jp1<=(width-1)))){
		sobelx_out =((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}
		else if(((im1>=0) && (jm1<0)) &&
                        ((ip1<=(height-1)) && (jp1>=0))){
		sobelx_out =((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}/********************************************************************************************************************/
		else if (((im1<0) && (jp1>(width-1))) &&
                         ((ip1>=0) && (jm1<=(width-1)))){/*Top-Right corner*/
		sobelx_out =((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1];

		sobely_out =((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1];
			}
		else if (((im1<0) && (jp1<=(width-1))) &&
                         ((ip1>=0) && (jm1<=(width-1)))){
		sobelx_out =((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}
		else if (((im1>=0) && (jp1>(width-1))) &&
                         ((ip1<=(height-1)) && (jm1<=(width-1)))){
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1];
			}/********************************************************************************************************************/
		else if (((ip1>(height-1)) && (jm1<0)) &&
                         ((im1<=(height-1)) && (jp1>=0))){/*Bottom-Left corner*/
		sobelx_out =((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2];

		sobely_out =((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2];
			}
		else if (((ip1>(height-1)) && (jm1>=0)) &&
                         ((im1<=(height-1)) && (jp1<=(width-1)))){
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2];
			}
		else if (((ip1<=(height-1)) && (jm1<0)) &&
                         ((im1<=(height-1)) && (jp1>=0))){
		sobelx_out =((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}/********************************************************************************************************************/
		else if (((ip1>(height-1)) && (jp1>(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){/*Bottom-Right corner*/
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1];
			}
		else if (((ip1>(height-1)) && (jp1<=(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2];
			}
		else if (((ip1<=(height-1)) && (jp1>(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1];
			}/********************************************************************************************************************/
		else{
		sobelx_out =((float)data2D[i-1][j-1][0])*Gx[0][0]+((float)data2D[i-1][j][0])*Gx[0][1]+((float)data2D[i-1][j+1][0])*Gx[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gx[1][0]+((float)data2D[i  ][j][0])*Gx[1][1]+((float)data2D[i  ][j+1][0])*Gx[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gx[2][0]+((float)data2D[i+1][j][0])*Gx[2][1]+((float)data2D[i+1][j+1][0])*Gx[2][2];

		sobely_out =((float)data2D[i-1][j-1][0])*Gy[0][0]+((float)data2D[i-1][j][0])*Gy[0][1]+((float)data2D[i-1][j+1][0])*Gy[0][2]+
                            ((float)data2D[i  ][j-1][0])*Gy[1][0]+((float)data2D[i  ][j][0])*Gy[1][1]+((float)data2D[i  ][j+1][0])*Gy[1][2]+
                            ((float)data2D[i+1][j-1][0])*Gy[2][0]+((float)data2D[i+1][j][0])*Gy[2][1]+((float)data2D[i+1][j+1][0])*Gy[2][2];
			}


		switch(type){
			float magnitude_x = 0.0, magnitude_y = 0.0;
			float abs_x = 0.0, abs_y = 0.0;
			case 0:/*Calculate magnitude using sqrt*/
				magnitude_x = (sobelx_out*sobelx_out);
				magnitude_y = (sobely_out*sobely_out);
				/*Edge strength*/
				magnitude_ttl = sqrt(magnitude_x + magnitude_y);

				sobel_data[i][j].magnitude = (unsigned char)magnitude_ttl;
				break;
			case 1:/*Calculate magnitude using abs*/
				abs_x = fabs(sobelx_out);
				abs_y = fabs(sobely_out);
				/*Edge strength*/
				magnitude_ttl = abs_x + abs_y;

				sobel_data[i][j].magnitude = (unsigned char)magnitude_ttl;
				break;
			case 2:/*Calculate direction*/
				if (sobelx_out == 0.0)
					theta = 2.0;
				else{
					angle = sobely_out / sobelx_out;
					
					if (angle < 0.0){
						if (angle < -2.41421356237)
							theta = 0.0;
						else{
							if (angle < -0.414213562373)
								theta = 1.0;
							else
								theta = 2.0;
							}
						}/*angle < 0.0*/
					else{
						if(angle > 2.41421356237)
							theta = 0.0;
						else{
							if (angle > 0.414213562373)
								theta = 3.0;
							else
								theta = 2.0;
							}
						}
					}

				sobel_data[i][j].direction = (unsigned char)theta;
				break;
			case 3:/*Calculate magnitude using sqrt and direction*/
				magnitude_x = (sobelx_out*sobelx_out);
				magnitude_y = (sobely_out*sobely_out);
				/*Edge strength*/
				magnitude_ttl = sqrt(magnitude_x + magnitude_y);
				sobel_data[i][j].magnitude = (unsigned char)magnitude_ttl;

				if (sobelx_out == 0.0)
					theta = 2.0;
				else{
					angle = sobely_out / sobelx_out;
					
					if (angle < 0.0){
						if (angle < -2.41421356237)
							theta = 0.0;
						else{
							if (angle < -0.414213562373)
								theta = 1.0;
							else
								theta = 2.0;
							}
						}/*angle < 0.0*/
					else{
						if(angle > 2.41421356237)
							theta = 0.0;
						else{
							if (angle > 0.414213562373)
								theta = 3.0;
							else
								theta = 2.0;
							}
						}
					}

				sobel_data[i][j].direction = (unsigned char)theta;
				break;
			}

		data_buffer[i][j] = sobel_data[i][j].magnitude;
		}
	}


for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = data_buffer[i][j];
		data2D[i][j][1] = data_buffer[i][j];
		data2D[i][j][2] = data_buffer[i][j];
		}
	}


for(i=0;i<height;i++)
	free(data_buffer[i]);
free(data_buffer);

return TRUE;
}


int non_maximum_suppression(int width, int height)
{
int i = 0, j = 0;

for(i=0;i<height;i++){
	for(j=0;j<width;j++){

		switch(sobel_data[i][j].direction){
			case 0:/*if the rounded edge direction angle is 0 degrees, checks the north and south directions*/
				if(((i-1)>=0) && ((i+1)<=(height-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j].magnitude) &&
					    (sobel_data[i][j].magnitude > sobel_data[i+1][j].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}/*
				else if(((i-1)>=0) && ((i+1)>(height-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}
				else if(((i-1)<0) && ((i+1)<=(height-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i+1][j].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}*/
				else{
					data2D[i][j][0] = 0;
					data2D[i][j][1] = 0;
					data2D[i][j][2] = 0;
					}
				break;
			case 1:/*if the rounded edge direction angle is 45 degrees, checks the northwest and southeast directions*/
				if (((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j-1].magnitude) && 
					    (sobel_data[i][j].magnitude > sobel_data[i+1][j+1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}/*
				else if ((((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)>(width-1))) ||
                                         (((i-1)>=0) && ((i+1)>(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))) ||
					 (((i-1)>=0) && ((i+1)>(height-1)) && ((j-1)>=0) && ((j+1)>(width-1)))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j-1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}
				else if ((((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)<0) && ((j+1)<=(width-1))) || 
					 (((i-1)<0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))) ||
					 (((i-1)<0) && ((i+1)<=(height-1)) && ((j-1)<0) && ((j+1)<=(width-1)))){
					if ((sobel_data[i][j].magnitude > sobel_data[i+1][j+1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}*/
				else{
					data2D[i][j][0] = 0;
					data2D[i][j][1] = 0;
					data2D[i][j][2] = 0;
					}
				break;
			case 2:/*if the rounded edge direction angle is 90 degrees, checks the east and west directions*/
				if (((j-1)>=0) && ((j+1)<=(width-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i][j+1].magnitude) && 
					    (sobel_data[i][j].magnitude > sobel_data[i][j-1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}/*
				else if(((j-1)>=0) && ((j+1)>(width-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i][j-1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}
				else if (((j-1)<0) && ((j+1)<=(width-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i][j+1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}*/
				else{
					data2D[i][j][0] = 0;
					data2D[i][j][1] = 0;
					data2D[i][j][2] = 0;
					}
				break;
			case 3:/*if the rounded edge direction angle is 135 degrees, checks the northeast and southwest directions*/
				if (((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j+1].magnitude) && 
					    (sobel_data[i][j].magnitude > sobel_data[i+1][j-1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}/*
				else if ((((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)>(width-1))) ||
					 (((i-1)<0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))) ||
					 (((i-1)<0) && ((i+1)<=(height-1)) && ((j-1)>=0) && ((j+1)>(width-1)))){
					if ((sobel_data[i][j].magnitude > sobel_data[i+1][j-1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}
				else if ((((i-1)>=0) && ((i+1)<=(height-1)) && ((j-1)<0) && ((j+1)<=(width-1))) ||
					 (((i-1)>=0) && ((i+1)>(height-1)) && ((j-1)>=0) && ((j+1)<=(width-1))) ||
					 (((i-1)>=0) && ((i+1)>(height-1)) && ((j-1)<0) && ((j+1)<=(width-1)))){
					if ((sobel_data[i][j].magnitude > sobel_data[i-1][j+1].magnitude)){
						data2D[i][j][0] = sobel_data[i][j].magnitude;
						data2D[i][j][1] = sobel_data[i][j].magnitude;
						data2D[i][j][2] = sobel_data[i][j].magnitude;
						}
					else{
						data2D[i][j][0] = 0;
						data2D[i][j][1] = 0;
						data2D[i][j][2] = 0;
						}
					}*/
				else{
					data2D[i][j][0] = 0;
					data2D[i][j][1] = 0;
					data2D[i][j][2] = 0;
					}
				break;
			}/*switch*/

		}
	}

return TRUE;
}

/*percentage of the high threshold value that the low threshold shall be set at*/
#define LOW_THRESHOLD_PERCENTAGE 0.8 
/*percentage of pixels that meet the high threshold - for example 0.15 will ensure 
that at least 15% of edge pixels are considered to meet the high threshold
*/
#define HIGH_THRESHOLD_PERCENTAGE 0.10 

int calculate_thresholds(int width, int height, int *high_thres, int *low_thres)
{
int i = 0, j = 0;
int histogram[256];
int pixels = 0, high_cutoff = 0;
memset(histogram, 0, sizeof(histogram));

for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		histogram[data2D[i][j][0]]++;
		}
	}

pixels = ((height*width) - histogram[0]) * HIGH_THRESHOLD_PERCENTAGE;
high_cutoff = 0;
i = 255;
while (high_cutoff < pixels){
	high_cutoff += histogram[i];
	i--;
	}

*high_thres = i;
i = 1;
while (histogram[i] == 0) {
	i++;
}
*low_thres = (*high_thres + i) * LOW_THRESHOLD_PERCENTAGE;

return TRUE;
}


int hysteresis_thresholding(int width, int height, int high_thres, int low_thres)
{
int i = 0, j = 0;

/* Allocate temporary memory to store the values for hysterisis.*/
unsigned char **data_buffer=NULL;
data_buffer = (unsigned char **)malloc(height * sizeof(unsigned char *));
if (data_buffer == NULL){
	printf("hysteresis_thresholding: Could not allocate %d bytes.\n", (height * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data_buffer[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (data_buffer[i] == NULL){
			printf("hysteresis_thresholding: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data_buffer[i][j] = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("hysteresis_thresholding: Allocated %d bytes.\n", (width * height * sizeof(unsigned char)));
	}


for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		if (data2D[i][j][0] >= high_thres){
			trace(width, height, i, j, low_thres, data_buffer);
			}
		}/*j*/
	}/*i*/


for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = data_buffer[i][j];
		data2D[i][j][1] = data_buffer[i][j];
		data2D[i][j][2] = data_buffer[i][j];
		}/*j*/
	}/*i*/

for(i=0;i<height;i++)
	free(data_buffer[i]);
free(data_buffer);

return TRUE;
}

int trace(int width, int height, int i, int j, int low_thres, unsigned char **data_out)
{
int i_off = 0, j_off = 0;

if (data_out[i][j] == 0){
	data_out[i][j] = 255;
	
	for (i_off=-1;i_off<=1;i_off++){
		for (j_off=-1;j_off<=1;j_off++){

			if (!(i == 0 && j_off == 0) && 
			    range(width, height, i + i_off, j + j_off) && 
			    data2D[(i + i_off)][(j + j_off)][0] >= low_thres){

				if (trace(width, height, (i + i_off), (j + j_off), low_thres, data_out))
					return TRUE;
				}

			}/*j_off*/
		}/*i_off*/
	return TRUE;
	}/*outer if*/
return FALSE;
}

int range(int width, int height, int i, int j)
{
if ((i < 0) || (i >= height))
	return FALSE;

if ((j < 0) || (j >= width))
	return FALSE;

return TRUE;
}

