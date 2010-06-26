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
float Gaussian_Filter[5][5] = {{2.0/115.0,  4.0/115.0,  5.0/115.0,  4.0/115.0, 2.0/115.0},
			       {4.0/115.0,  9.0/115.0, 12.0/115.0,  9.0/115.0, 4.0/115.0},
			       {5.0/115.0, 12.0/115.0, 15.0/115.0, 12.0/115.0, 5.0/115.0},
			       {4.0/115.0,  9.0/115.0, 12.0/115.0,  9.0/115.0, 4.0/115.0},
			       {2.0/115.0,  4.0/115.0,  5.0/115.0,  4.0/115.0, 2.0/115.0}};

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

/* Allocate temporary memory to store the filtered data from greyscale image.*/
unsigned char **buffer=NULL;
buffer = (unsigned char **)malloc(height * sizeof(unsigned char *));
if (buffer == NULL){
	printf("noise_reduction: Could not allocate %d bytes.\n", (height * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		buffer[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (buffer[i] == NULL){
			printf("noise_reduction: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				buffer[i][j] = 0;
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
		buffer[i][j] = grey;
		}
	}

for(i=0;i<height;i++){
	int im2 = (i-2), im1 = (i-1), ip2 = (i+2), ip1 = (i+1);
	/*if (i>2) continue;*/
	for(j=0;j<width;j++){
		int jm2 = (j-2), jm1 = (j-1), jp2 = (j+2), jp1 = (j+1);
		unsigned char filter_out = 0;
		
		/*printf("-[%d %d] %d +[%d %d] -[%d %d] %d +[%d %d]\n", im2, im1, i, ip1, ip2, jm2, jm1, j, jp1, jp2);*/
		/*printf("Grey[%u %u %u]\n", data2D[i][j][0], data2D[i][j][1], data2D[i][j][2]);*/

		if (((im2<0) && (im1<0) && (jm2<0) && (jm1<0)) &&
                    ((ip2>=0) && (ip1>=0) && (jp2>=0) && (jp1>=0))){/*Top-Left corner*/
		/*printf("((im2<0) && (im1<0) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2<0) && (im1<0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1>=0) && (jp2>=0) && (jp1<=(width-1)))){
		/*printf("((im2<0) && (im1<0) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2<0) && (im1<0) && (jm2>=0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1>=0) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		/*printf("((im2<0) && (im1<0) && (jm2>=0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2<0) && (im1>=0) && (jm2<0) && (jm1<0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((im2<0) && (im1>=0) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2>=0) && (im1>=0) && (jm2<0) && (jm1<0)) &&
                        ((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((im2>=0) && (im1>=0) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2>=0) && (im1>=0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((im2>=0) && (im1>=0) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2<0) && (im1>=0) && (jm2<0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2>=0) && (jp1<=(width-1)))){
		/*printf("((im2<0) && (im1>=0) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if(((im2<0) && (im1>=0) && (jm2>=0) && (jm1>=0)) &&
                        ((ip2>=0) && (ip1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		/*printf("((im2<0) && (im1>=0) && (jm2>=0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);

			}/********************************************************************************************************************/
		else if (((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){/*Top-Right corner*/
		/*printf("((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]);
			}
		else if (((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2<0) && (im1<0) && (jp2>(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]);
			}
		else if (((im2<0) && (im1<0) && (jp2<=(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2>=0) && (jm1>=0))){
		/*printf("((im2<0) && (im1<0) && (jp2<=(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if (((im2<0) && (im1>=0) && (jp2<=(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2<0) && (im1>=0) && (jp2<=(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);

			}
		else if (((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]);

			}
		else if (((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]);
			}
		else if (((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2>=0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]);

			}
		else if (((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((ip2>=0) && (ip1>=0) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((im2<0) && (im1>=0) && (jp2>(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]);
			}/********************************************************************************************************************/
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1<0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){/*Bottom-Left corner*/
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]);
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]);
			}/**/
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jm2>=0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jm2>=0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]);
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1<0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]);
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((ip2>(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]);
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jm2>=0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))){
		/*printf("((ip2>(height-1)) && (ip1<=(height-1)) && (jm2>=0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]);
			}
		else if (((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1<0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1<0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}
		else if (((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1>=0)) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jp2>=0) && (jp1>=0))){
		/*printf("((ip2<=(height-1)) && (ip1<=(height-1)) && (jm2<0) && (jm1>=0))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}/********************************************************************************************************************/
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){/*Bottom-Right corner*/
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]);
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jp2>(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]);
			}
		else if (((ip2>(height-1)) && (ip1>(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((ip2>(height-1)) && (ip1>(height-1)) && (jp2<=(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]);
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]);
			}
		else if (((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1<=(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2>=0) && (jm1>=0))){
		/*printf("((ip2>(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1<=(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]);
			}
		else if (((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1>(width-1))) &&
                         ((im2<=(height-1)) && (im1<=(height-1)) && (jm2<=(width-1)) && (jm1<=(width-1)))){
		/*printf("((ip2<=(height-1)) && (ip1<=(height-1)) && (jp2>(width-1)) && (jp1>(width-1)))\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]);
			}/********************************************************************************************************************/
		else{
		/*printf("else\n");*/
		filter_out = (unsigned char)(
			      ((float)buffer[i-2][j-2])*Gaussian_Filter[0][0]+((float)buffer[i-2][j-1])*Gaussian_Filter[0][1]+ 
                              ((float)buffer[i-2][j  ])*Gaussian_Filter[0][2]+((float)buffer[i-2][j+1])*Gaussian_Filter[0][3]+
                              ((float)buffer[i-2][j+2])*Gaussian_Filter[0][4]+
			      ((float)buffer[i-1][j-2])*Gaussian_Filter[1][0]+((float)buffer[i-1][j-1])*Gaussian_Filter[1][1]+
                              ((float)buffer[i-1][j  ])*Gaussian_Filter[1][2]+((float)buffer[i-1][j+1])*Gaussian_Filter[1][3]+ 
                              ((float)buffer[i-1][j+2])*Gaussian_Filter[1][4]+
			      ((float)buffer[i  ][j-2])*Gaussian_Filter[2][0]+((float)buffer[i  ][j-1])*Gaussian_Filter[2][1]+
                              ((float)buffer[i  ][j  ])*Gaussian_Filter[2][2]+((float)buffer[i  ][j+1])*Gaussian_Filter[2][3]+ 
                              ((float)buffer[i  ][j+2])*Gaussian_Filter[2][4]+
			      ((float)buffer[i+1][j-2])*Gaussian_Filter[3][0]+((float)buffer[i+1][j-1])*Gaussian_Filter[3][1]+
                              ((float)buffer[i+1][j  ])*Gaussian_Filter[3][2]+((float)buffer[i+1][j+1])*Gaussian_Filter[3][3]+ 
                              ((float)buffer[i+1][j+2])*Gaussian_Filter[3][4]+
			      ((float)buffer[i+2][j-2])*Gaussian_Filter[4][0]+((float)buffer[i+2][j-1])*Gaussian_Filter[4][1]+
                              ((float)buffer[i+2][j  ])*Gaussian_Filter[4][2]+((float)buffer[i+2][j+1])*Gaussian_Filter[4][3]+ 
                              ((float)buffer[i+2][j+2])*Gaussian_Filter[4][4]);
			}

		/*printf("%03u ", filter_out);*/
		data_buffer[i][j] = filter_out;
		}
	/*printf("\n");*/
	}


for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		data2D[i][j][0] = data_buffer[i][j];
		data2D[i][j][1] = data_buffer[i][j];
		data2D[i][j][2] = data_buffer[i][j];
		}
	}


for(i=0;i<height;i++)
	free(buffer[i]);
free(buffer);

for(i=0;i<height;i++)
	free(data_buffer[i]);
free(data_buffer);

return TRUE;
}

