/* ############################################################################
Name           : alg3.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm 3.

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */

#define __ALG3_C__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "alg3.h"
#include "alcon2009.h"/*For save_ppm function*/

RGB **data2D_RGB;
CIELab **data2D_CIELAB;

int transform_1D_to_2D_RGB(unsigned char *image_data, int width, int height)
{
unsigned int i = 0, j = 0;

/* Allocate memory for subimage data*/
data2D_RGB = (RGB **)malloc(height * sizeof(RGB *));
if (data2D_RGB == NULL){
	printf("transform_1D_to_2D: Could not allocate %d bytes.\n", (height * sizeof(RGB *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data2D_RGB[i] = (RGB *)malloc(width * sizeof(RGB));
		if (data2D_RGB[i] == NULL){
			printf("transform_1D_to_2D: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(RGB)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data2D_RGB[i][j].r = 0;
				data2D_RGB[i][j].g = 0;
				data2D_RGB[i][j].b = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("transform_1D_to_2D: Allocated %d bytes.\n", (width * height * sizeof(RGB)));
	}


for (i=0;i<height;i++){
	for(j=0;j<width;j++){
		register unsigned int idx = (j + i*width)*3;
		data2D_RGB[i][j].r = image_data[idx + 0];
		data2D_RGB[i][j].g = image_data[idx + 1];
		data2D_RGB[i][j].b = image_data[idx + 2];
		}
	}

return TRUE;
}

int allocate_mem_CIELAB(int width, int height)
{
unsigned int i = 0, j = 0;

/* Allocate memory for CIELAB data*/
data2D_CIELAB = (CIELab **)malloc(height * sizeof(CIELab *));
if (data2D_CIELAB == NULL){
	printf("allocate_mem_CIELAB: Could not allocate %d bytes.\n", (height * sizeof(CIELab *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data2D_CIELAB[i] = (CIELab *)malloc(width * sizeof(CIELab));
		if (data2D_CIELAB[i] == NULL){
			printf("allocate_mem_CIELAB: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(CIELab)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data2D_CIELAB[i][j].C = 0;
				data2D_CIELAB[i][j].L = 0.0;
				data2D_CIELAB[i][j].a = 0.0;
				data2D_CIELAB[i][j].b = 0.0;
				}/*for j*/
			}
		}/*for i*/
	printf("allocate_mem_CIELAB: Allocated %d bytes.\n", (width * height * sizeof(CIELab)));
	}


return TRUE;
}

/*
Source: http://inkscape.modevia.com/doxygen/html/classorg_1_1siox_1_1CieLab.php
*/
int RGB_to_CIELAB(RGB in, CIELab *out)
{
out->C = 0;
out->L = 0.0;
out->a = 0.0;
out->b = 0.0;

float fr = ((float)in.r) / 255.0;
float fg = ((float)in.g) / 255.0;
float fb = ((float)in.b) / 255.0;

if (fr > 0.04045)
	fr = (float) pow((fr + 0.055) / 1.055, 2.4);
else
	fr = fr / 12.92;

if (fg > 0.04045)
	fg = (float) pow((fg + 0.055) / 1.055, 2.4);
else
	fg = fg / 12.92;

if (fb > 0.04045)
	fb = (float) pow((fb + 0.055) / 1.055, 2.4);
else
	fb = fb / 12.92;


/*Use white = D65*/
const float x = fr * 0.4124 + fg * 0.3576 + fb * 0.1805;
const float y = fr * 0.2126 + fg * 0.7152 + fb * 0.0722;
const float z = fr * 0.0193 + fg * 0.1192 + fb * 0.9505;

float vx = x / 0.95047;
float vy = y;
float vz = z / 1.08883;


if (vx > 0.008856)
	vx = (float) pow(vx, 0.3333);
else
	vx = (7.787 * vx) + (16.0 / 116.0);

if (vy > 0.008856)
	vy = (float) pow(vy, 0.3333);
else
	vy = (7.787 * vy) + (16.0 / 116.0);

if (vz > 0.008856)
	vz = (float) pow(vz, 0.3333);
else
	vz = (7.787 * vz) + (16.0 / 116.0);

out->C = 0;
out->L = 116.0 * vy - 16.0;
out->a = 500.0 * (vx - vy);
out->b = 200.0 * (vy - vz);


return TRUE;
}

int convert_RGB_to_CIELAB(int width, int height)
{
unsigned int i = 0, j = 0;
CIELab temp;
memset(&temp, 0, sizeof(temp));

for (i=0;i<height;i++){
	for(j=0;j<width;j++){
		RGB_to_CIELab(data2D_RGB[i][j], &temp);
		data2D_CIELAB[i][j].C = temp.C;
		data2D_CIELAB[i][j].L = temp.L;
		data2D_CIELAB[i][j].a = temp.a;
		data2D_CIELAB[i][j].b = temp.b;
		}
	}

return TRUE;
}

int Sobel_CIELAB(int width, int height)
{
unsigned int i = 0, j = 0;

/*Sobel kernel functions*/
float Gx[3][3] = {{-1.0, 0.0, 1.0}, 
		  {-2.0, 0.0, 2.0}, 
		  {-1.0, 0.0, 1.0}};

float Gy[3][3] = {{ 1.0,  2.0,  1.0}, 
		  { 0.0,  0.0,  0.0}, 
		  {-1.0, -2.0, -1.0}};


/* Allocate temporary memory to store the values after sobel operator has passed and gradient has been computed.*/
float **data_buffer=NULL;
data_buffer = (float **)malloc(height * sizeof(float *));
if (data_buffer == NULL){
	printf("Sobel_CIELAB: Could not allocate %d bytes.\n", (height * sizeof(float *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data_buffer[i] = (float *)malloc(width * sizeof(float));
		if (data_buffer[i] == NULL){
			printf("Sobel_CIELAB: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(float)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data_buffer[i][j] = 0.0;
				}/*for j*/
			}
		}/*for i*/
	printf("Sobel_CIELAB: Allocated %d bytes.\n", (width * height * sizeof(float)));
	}


for(i=0;i<height;i++){
	int im1 = (i-1), ip1 = (i+1);
	for(j=0;j<width;j++){
		int jm1 = (j-1), jp1 = (j+1);
		float dLdx = 0.0, dLdy = 0.0;
		float dadx = 0.0, dady = 0.0;
		float dbdx = 0.0, dbdy = 0.0;
		float q = 0.0, t = 0.0, h = 0.0, lamda = 0.0, qplush = 0.0;

		if (((im1<0) && (jm1<0)) &&
                    ((ip1>=0) &&(jp1>=0))){/*Top-Left corner*/
		dLdx =data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
		      data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
		      data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
		      data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
		      data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
		      data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
		      data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}
		else if(((im1<0) && (jm1>=0)) &&
                        ((ip1>=0) && (jp1<=(width-1)))){
		dLdx =data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
		      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
		      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
		      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
		      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
		      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
		      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}
		else if(((im1>=0) && (jm1<0)) &&
                        ((ip1<=(height-1)) && (jp1>=0))){
		dLdx =data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
                      data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
                      data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
                      data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
                      data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
                      data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
                      data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}/********************************************************************************************************************/
		else if (((im1<0) && (jp1>(width-1))) &&
                         ((ip1>=0) && (jm1<=(width-1)))){/*Top-Right corner*/
		dLdx =data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1];
		dLdy =data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1];
		dadx =data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1];
		dady =data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1];
		dbdx =data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1];
		dbdy =data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1];
			}
		else if (((im1<0) && (jp1<=(width-1))) &&
                         ((ip1>=0) && (jm1<=(width-1)))){
		dLdx =data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}
		else if (((im1>=0) && (jp1>(width-1))) &&
                         ((ip1<=(height-1)) && (jm1<=(width-1)))){
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1];
			}/********************************************************************************************************************/
		else if (((ip1>(height-1)) && (jm1<0)) &&
                         ((im1<=(height-1)) && (jp1>=0))){/*Bottom-Left corner*/
		dLdx =data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2];
		dLdy =data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2];
		dadx =data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2];
		dady =data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2];
		dbdx =data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2];
		dbdy =data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2];
			}
		else if (((ip1>(height-1)) && (jm1>=0)) &&
                         ((im1<=(height-1)) && (jp1<=(width-1)))){
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2];
			}
		else if (((ip1<=(height-1)) && (jm1<0)) &&
                         ((im1<=(height-1)) && (jp1>=0))){
		dLdx =data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
                      data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
                      data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
                      data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
                      data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
                      data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
                      data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}/********************************************************************************************************************/
		else if (((ip1>(height-1)) && (jp1>(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){/*Bottom-Right corner*/
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1];
			}
		else if (((ip1>(height-1)) && (jp1<=(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2];
			}
		else if (((ip1<=(height-1)) && (jp1>(width-1))) &&
                         ((im1<=(height-1)) && (jm1<=(width-1)))){
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+
                      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1];
			}/********************************************************************************************************************/
		else{
		dLdx =data2D_CIELAB[i-1][j-1].L*Gx[0][0]+data2D_CIELAB[i-1][j].L*Gx[0][1]+data2D_CIELAB[i-1][j+1].L*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gx[1][0]+data2D_CIELAB[i  ][j].L*Gx[1][1]+data2D_CIELAB[i  ][j+1].L*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].L*Gx[2][0]+data2D_CIELAB[i+1][j].L*Gx[2][1]+data2D_CIELAB[i+1][j+1].L*Gx[2][2];
		dLdy =data2D_CIELAB[i-1][j-1].L*Gy[0][0]+data2D_CIELAB[i-1][j].L*Gy[0][1]+data2D_CIELAB[i-1][j+1].L*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].L*Gy[1][0]+data2D_CIELAB[i  ][j].L*Gy[1][1]+data2D_CIELAB[i  ][j+1].L*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].L*Gy[2][0]+data2D_CIELAB[i+1][j].L*Gy[2][1]+data2D_CIELAB[i+1][j+1].L*Gy[2][2];
		dadx =data2D_CIELAB[i-1][j-1].a*Gx[0][0]+data2D_CIELAB[i-1][j].a*Gx[0][1]+data2D_CIELAB[i-1][j+1].a*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gx[1][0]+data2D_CIELAB[i  ][j].a*Gx[1][1]+data2D_CIELAB[i  ][j+1].a*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].a*Gx[2][0]+data2D_CIELAB[i+1][j].a*Gx[2][1]+data2D_CIELAB[i+1][j+1].a*Gx[2][2];
		dady =data2D_CIELAB[i-1][j-1].a*Gy[0][0]+data2D_CIELAB[i-1][j].a*Gy[0][1]+data2D_CIELAB[i-1][j+1].a*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].a*Gy[1][0]+data2D_CIELAB[i  ][j].a*Gy[1][1]+data2D_CIELAB[i  ][j+1].a*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].a*Gy[2][0]+data2D_CIELAB[i+1][j].a*Gy[2][1]+data2D_CIELAB[i+1][j+1].a*Gy[2][2];
		dbdx =data2D_CIELAB[i-1][j-1].b*Gx[0][0]+data2D_CIELAB[i-1][j].b*Gx[0][1]+data2D_CIELAB[i-1][j+1].b*Gx[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gx[1][0]+data2D_CIELAB[i  ][j].b*Gx[1][1]+data2D_CIELAB[i  ][j+1].b*Gx[1][2]+
                      data2D_CIELAB[i+1][j-1].b*Gx[2][0]+data2D_CIELAB[i+1][j].b*Gx[2][1]+data2D_CIELAB[i+1][j+1].b*Gx[2][2];
		dbdy =data2D_CIELAB[i-1][j-1].b*Gy[0][0]+data2D_CIELAB[i-1][j].b*Gy[0][1]+data2D_CIELAB[i-1][j+1].b*Gy[0][2]+
                      data2D_CIELAB[i  ][j-1].b*Gy[1][0]+data2D_CIELAB[i  ][j].b*Gy[1][1]+data2D_CIELAB[i  ][j+1].b*Gy[1][2]+
                      data2D_CIELAB[i+1][j-1].b*Gy[2][0]+data2D_CIELAB[i+1][j].b*Gy[2][1]+data2D_CIELAB[i+1][j+1].b*Gy[2][2];
			}

		q = (dLdx*dLdx) + (dadx*dadx) + (dbdx*dbdx);
		t = (dLdx*dLdy) + (dadx*dady) + (dbdx*dbdy);
		h = (dLdy*dLdy) + (dady*dady) + (dbdy*dbdy);
		qplush = (q + h);
		lamda = (qplush + sqrt((qplush*qplush) - 4*(q*h - (t*t)))) / 2;

		data_buffer[i][j] = sqrt(lamda);
		printf("%.3f ", data_buffer[i][j]);
		}
	printf("\n");
	}



for(i=0;i<height;i++)
	free(data_buffer[i]);
free(data_buffer);

return TRUE;
}

