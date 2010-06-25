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
unsigned int i = 0, j = 0;
float Gaussian_Filter[5][5] = {{2/115,  4/115,  5/115,  4/115, 2/115},
			       {4/115,  9/115, 12/115,  9/115, 4/115},
			       {5/115, 12/115, 15/115, 12/115, 5/115},
			       {4/115,  9/115, 12/115,  9/115, 4/115},
			       {2/115,  4/115,  5/115,  4/115, 2/115}};


printf("1\n");
for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		unsigned char grey = 0, filter_out = 0;
		grey = GREYSCALE1(data2D[i][j][0], data2D[i][j][1], data2D[i][j][2]);
		data2D[i][j][0] = data2D[i][j][1] = data2D[i][j][2] = grey;
		
		printf("%u %u\n", i, j);

		if (((i-2)<0) && ((i-1)<0) && ((j-2)<0) && ((j-1)<0)){/*Top-Left corner*/
		printf("(((i-2)<0) && ((i-1)<0) && ((j-2)<0) && ((j-1)<0))\n");
		filter_out = (unsigned char)((float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if(((i-2)<0) && ((i-1)<0) && ((j-2)<0) && ((j-1)>=0)){
		printf("(((i-2)<0) && ((i-1)<0) && ((j-2)<0) && ((j-1)>=0))\n");
		filter_out = (unsigned char)((float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if(((i-2)<0) && ((i-1)<0) && ((j-2)>=0) && ((j-1)>=0)){
		printf("(((i-2)<0) && ((i-1)<0) && ((j-2)>=0) && ((j-1)>=0))\n");
		filter_out = (unsigned char)((float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if(((i-2)<0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)<0)){
		printf("(((i-2)<0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)<0))\n");
		filter_out = (unsigned char)((float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if(((i-2)>=0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)<0)){
		printf("(((i-2)>=0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)<0))\n");
		filter_out = (unsigned char)((float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if(((i-2)<0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)>=0)){
		printf("(((i-2)<0) && ((i-1)>=0) && ((j-2)<0) && ((j-1)>=0))\n");
		filter_out = (unsigned char)((float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}/********************************************************************************************************************/
		else if (((i-2)<0) && ((i-1)<0) && ((j+2)>(width-1)) && ((j+1)>(width-1))){/*Top-Right corner*/
		filter_out = (unsigned char)((float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]);
			}
		else if (((i-2)<0) && ((i-1)<0) && ((j+2)>(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]);
			}
		else if (((i-2)<0) && ((i-1)<0) && ((j+2)<=(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if (((i-2)<0) && ((i-1)>=0) && ((j+2)>(width-1)) && ((j+1)>(width-1))){
		filter_out = (unsigned char)((float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]);

			}
		else if (((i-2)>=0) && ((i-1)>=0) && ((j+2)>(width-1)) && ((j+1)>(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]);
			}
		else if (((i-2)<0) && ((i-1)>=0) && ((j+2)>(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]);
			}/********************************************************************************************************************/
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j-2)<0) && ((j-1)<0)){/*Bottom-Left corner*/
		filter_out = (unsigned char)((float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]);
			}
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j-2)<0) && ((j-1)>=0)){
		filter_out = (unsigned char)((float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]);
			}
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j-2)>=0) && ((j-1)>=0)){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]);
			}
		else if (((i+2)>(height-1)) && ((i+1)<=(height-1)) && ((j-2)<0) && ((j-1)<0)){
		filter_out = (unsigned char)((float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]);
			}
		else if (((i+2)<=(height-1)) && ((i+1)<=(height-1)) && ((j-2)<0) && ((j-1)<0)){
		filter_out = (unsigned char)((float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}
		else if (((i+2)>(height-1)) && ((i+1)<=(height-1)) && ((j-2)<0) && ((j-1)>=0)){
		filter_out = (unsigned char)((float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]);
			}/********************************************************************************************************************/
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j+2)>(width-1)) && ((j+1)>(width-1))){/*Bottom-Right corner*/
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]);
			}
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j+2)>(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]);
			}
		else if (((i+2)>(height-1)) && ((i+1)>(height-1)) && ((j+2)<=(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]);
			}
		else if (((i+2)>(height-1)) && ((i+1)<=(height-1)) && ((j+2)>(width-1)) && ((j+1)>(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]);
			}
		else if (((i+2)<=(height-1)) && ((i+1)<=(height-1)) && ((j+2)>(width-1)) && ((j+1)>(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]);
			}
		else if (((i+2)>(height-1)) && ((i+1)<=(height-1)) && ((j+2)>(width-1)) && ((j+1)<=(width-1))){
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]);
			}/********************************************************************************************************************/
		else{
		printf("else\n");
		filter_out = (unsigned char)((float)data2D[i-2][j-2][0]*Gaussian_Filter[0][0]+(float)data2D[i-2][j-1][0]*Gaussian_Filter[0][1]+ 
                              (float)data2D[i-2][j  ][0]*Gaussian_Filter[0][2]+(float)data2D[i-2][j+1][0]*Gaussian_Filter[0][3]+
                              (float)data2D[i-2][j+2][0]*Gaussian_Filter[0][4]+
			      (float)data2D[i-1][j-2][0]*Gaussian_Filter[1][0]+(float)data2D[i-1][j-1][0]*Gaussian_Filter[1][1]+
                              (float)data2D[i-1][j  ][0]*Gaussian_Filter[1][2]+(float)data2D[i-1][j+1][0]*Gaussian_Filter[1][3]+ 
                              (float)data2D[i-1][j+2][0]*Gaussian_Filter[1][4]+
			      (float)data2D[i  ][j-2][0]*Gaussian_Filter[2][0]+(float)data2D[i  ][j-1][0]*Gaussian_Filter[2][1]+
                              (float)data2D[i  ][j  ][0]*Gaussian_Filter[2][2]+(float)data2D[i  ][j+1][0]*Gaussian_Filter[2][3]+ 
                              (float)data2D[i  ][j+2][0]*Gaussian_Filter[2][4]+
			      (float)data2D[i+1][j-2][0]*Gaussian_Filter[3][0]+(float)data2D[i+1][j-1][0]*Gaussian_Filter[3][1]+
                              (float)data2D[i+1][j  ][0]*Gaussian_Filter[3][2]+(float)data2D[i+1][j+1][0]*Gaussian_Filter[3][3]+ 
                              (float)data2D[i+1][j+2][0]*Gaussian_Filter[3][4]+
			      (float)data2D[i+2][j-2][0]*Gaussian_Filter[4][0]+(float)data2D[i+2][j-1][0]*Gaussian_Filter[4][1]+
                              (float)data2D[i+2][j  ][0]*Gaussian_Filter[4][2]+(float)data2D[i+2][j+1][0]*Gaussian_Filter[4][3]+ 
                              (float)data2D[i+2][j+2][0]*Gaussian_Filter[4][4]);
			}




		data2D[i][j][0] = data2D[i][j][1] = data2D[i][j][2] = filter_out;
		}
	}

return TRUE;
}

