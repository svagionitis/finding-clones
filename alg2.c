/* ############################################################################
Name           : alg2.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm 2.

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

int Sobel_operators(int width, int height)
{
int i = 0, j = 0;

float Gx[3][3] = {{-1.0, 0.0, 1.0}, 
		  {-2.0, 0.0, 2.0}, 
		  {-1.0, 0.0, 1.0}};

float Gy[3][3] = {{ 1.0,  2.0,  1.0}, 
		  { 0.0,  0.0,  0.0}, 
		  {-1.0, -2.0, -1.0}};

/* Allocate temporary memory to store the values after sobel operator has passed.*/
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
	printf("Sobel_operators: Allocated %d bytes.\n", (width * height * sizeof(unsigned char)));
	}

for(i=0;i<height;i++){
	int im1 = (i-1), ip1 = (i+1);
	for(j=0;j<width;j++){
		int jm1 = (j-1), jp1 = (j+1);
		unsigned char filter_out = 0.0;
		float sobelx_out = 0.0, sobely_out = 0.0;
		float magnitude_x = 0.0, magnitude_y = 0.0, magnitude_ttl = 0.0;
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

		magnitude_x = pow(sobelx_out,2);
		magnitude_y = pow(sobely_out,2);
		/*Edge strength*/
		magnitude_ttl = sqrt(magnitude_x + magnitude_y);

/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/
		float abs_x = fabs(sobelx_out);
		float abs_y = fabs(sobely_out);
		if (abs_x == 0.0)
			theta = 0.0;
		else{
			if (abs_y == 0.0)
				theta = 0.0;
			else{
				/*atan returns radians*/
				angle = atan(abs_y / abs_x);
				if (abs_x >= 0.0){
					if (abs_y >= 0.0)
						theta = angle;
					else
						theta = (2.0*PI-angle);
					}
				else{
					if (abs_y >= 0.0)
						theta = PI-angle;
					else
						theta = PI+angle;
					}

				}
			}
/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\*/

		printf("%f\n", DEG(theta));
		
		filter_out = (unsigned char)magnitude_ttl;
		/*filter_out = (unsigned char)theta;*/

		data_buffer[i][j] = filter_out;
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
