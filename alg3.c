/* ############################################################################
Name           : alg3.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm 3. Implementation of the algorithm 
                 described in 
                 L. G. Ugarriza, E. Saber, V. Amuso, M. Shaw, and R. Bhskar, 
                 “Automatic image segmentation by dynamic region growth and 
                  multiresolution merging,” Journal of Image Processing, 
                  vol. 99, no. 99, 2009

Procedure                          Description
================================== ============================================
transform_1D_to_2D_RGB             Transform 1D data to a 2D for better 
                                   handling
allocate_mem_data_CIELAB           Allocate memory for 2D data in CIE L*a*b* 
                                   color space.
RGB_to_CIELAB                      Transform RGB values to CIE L*a*b* values.
convert_RGB_to_CIELAB              Convert all image RGB data to CIE L*a*b* 
                                   data
first_derivative_CIELAB            Calculate gradient values using kernel 
                                   functions(Sobel, Prewitt).
calculate_histogram_of_gradient    Calculate the histogram of gradient values.


Globals        Type                Description
============== =================== ============================================
data2D_RGB     RGB **              Image data in RGB color space.
data2D_CIELAB  CIELab **           Image data in CIE L*a*b* color space.
gradient_Map   unsigned int **     Gradient values.
hist_gradient  unsigned int *      Histogram of gradient map.

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
unsigned int **gradient_Map;
unsigned int *hist_gradient;

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
data2D_RGB(OUT)       RGB **              Image data in RGB color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int transform_1D_to_2D_RGB(unsigned char *image_data, int width, int height)
{
unsigned int i = 0, j = 0;

/* Allocate memory for subimage data*/
data2D_RGB = (RGB **)malloc(height * sizeof(RGB *));
if (data2D_RGB == NULL){
	printf("transform_1D_to_2D_RGB: Could not allocate %d bytes.\n", (height * sizeof(RGB *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data2D_RGB[i] = (RGB *)malloc(width * sizeof(RGB));
		if (data2D_RGB[i] == NULL){
			printf("transform_1D_to_2D_RGB: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(RGB)), i);
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
	printf("transform_1D_to_2D_RGB: Allocated %d bytes.\n", (width * height * sizeof(RGB)));
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


/* ############################################################################
Name           : allocate_mem_data_CIELAB
Description    : Allocate memory for 2D data in CIE L*a*b* color space.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================
data2D_CIELAB(OUT)    CIELab **           Image data in CIE L*a*b* color space.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int allocate_mem_data_CIELAB(int width, int height)
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


/* ############################################################################
Name           : RGB_to_CIELAB
Description    : Transform RGB values to CIE L*a*b* values.

(Source: http://inkscape.modevia.com/doxygen/html/classorg_1_1siox_1_1CieLab.php)

Arguments             Type                Description
===================== =================== =====================================
in(IN)                RGB                 RGB value.
out(OUT)              CIELab *            CIE L*a*b* value.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================

Locals                Type                Description
===================== =================== =====================================

############################################################################ */
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
	vx = (float) pow(vx, 0.3333);/*cube root*/
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
		RGB_to_CIELAB(data2D_RGB[i][j], &temp);
		data2D_CIELAB[i][j].C = temp.C;
		data2D_CIELAB[i][j].L = temp.L;
		data2D_CIELAB[i][j].a = temp.a;
		data2D_CIELAB[i][j].b = temp.b;
		}
	}

return TRUE;
}


/* ############################################################################
Name           : first_derivative_CIELAB
Description    : Calculate gradient values using kernel 
                 functions(Sobel, Prewitt).

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       If 0 Sobel kernel function
                                          If 1 Prewitt kernel function
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
max_gradient(OUT)     unsigned int *      Max gradient value in the image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
data2D_CIELAB(IN)     CIELab **           Image data in CIE L*a*b* color space.
gradient_Map(OUT)     unsigned int **     Gradient values.

Locals                Type                Description
===================== =================== =====================================

############################################################################ */
int first_derivative_CIELAB(unsigned char type, int width, int height, unsigned int *max_gradient)
{
unsigned int i = 0, j = 0;
float Gx[3][3], Gy[3][3];
memset(Gx, 0.0, sizeof(Gx));
memset(Gy, 0.0, sizeof(Gy));


switch(type){
	case 0:
		/*Sobel kernel functions*/
		Gx[0][0] = -1.0;Gx[0][1] = 0.0;Gx[0][2] = 1.0;
		Gx[1][0] = -2.0;Gx[1][1] = 0.0;Gx[1][2] = 2.0;
		Gx[2][0] = -1.0;Gx[2][1] = 0.0;Gx[2][2] = 1.0;

		Gy[0][0] =  1.0;Gy[0][1] =  2.0;Gy[0][2] =  1.0;
		Gy[1][0] =  0.0;Gy[1][1] =  0.0;Gy[1][2] =  0.0;
		Gy[2][0] = -1.0;Gy[2][1] = -2.0;Gy[2][2] = -1.0;
		break;
	case 1:
		/*Prewitt kernel functions*/
		Gx[0][0] = -1.0;Gx[0][1] = 0.0;Gx[0][2] = 1.0;
		Gx[1][0] = -1.0;Gx[1][1] = 0.0;Gx[1][2] = 1.0;
		Gx[2][0] = -1.0;Gx[2][1] = 0.0;Gx[2][2] = 1.0;

		Gy[0][0] =  1.0;Gy[0][1] =  1.0;Gy[0][2] =  1.0;
		Gy[1][0] =  0.0;Gy[1][1] =  0.0;Gy[1][2] =  0.0;
		Gy[2][0] = -1.0;Gy[2][1] = -1.0;Gy[2][2] = -1.0;
		break;
}


/* Allocate memory to store the values after sobel operator has passed and gradient has been computed.*/
gradient_Map = (unsigned int **)malloc(height * sizeof(unsigned int *));
if (gradient_Map == NULL){
	printf("first_derivative_CIELAB: Could not allocate %d bytes.\n", (height * sizeof(unsigned int *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		gradient_Map[i] = (unsigned int *)malloc(width * sizeof(unsigned int));
		if (gradient_Map[i] == NULL){
			printf("first_derivative_CIELAB: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned int)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				gradient_Map[i][j] = 0;
				}/*for j*/
			}
		}/*for i*/
	printf("first_derivative_CIELAB: Allocated %d bytes.\n", (width * height * sizeof(unsigned int)));
	}


(*max_gradient) = 0;
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
		lamda = (qplush + sqrt((qplush*qplush) - 4.0*(q*h - (t*t)))) / 2.0;

		gradient_Map[i][j] = (unsigned int)sqrt(lamda);

		/*Find max value*/
		if (gradient_Map[i][j] >= (*max_gradient))
			(*max_gradient) = gradient_Map[i][j];

		printf("%03u ", gradient_Map[i][j]);
		}
	printf("\n");
	}


return TRUE;
}


/* ############################################################################
Name           : calculate_histogram_of_gradient
Description    : 

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
max_gradient(IN)      unsigned int        Max gradient value in the image.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
hist_gradient(OUT)    unsigned int *      Histogram of gradient map.

Locals                Type                Description
===================== =================== =====================================

############################################################################ */
int calculate_histogram_of_gradient(int width, int height, unsigned int max_gradient)
{
unsigned int i = 0, j = 0;

hist_gradient = (unsigned int *)malloc((max_gradient+1) * sizeof(unsigned int));
if (hist_gradient == NULL){
	printf("calculate_histogram_of_gradient: Could not allocate %d bytes.\n", (max_gradient+1) * sizeof(unsigned int));
	return FALSE;
	}
else{
	for (i=0;i<(max_gradient+1);i++)
		hist_gradient[i] = 0;
	}

for(i=0;i<height;i++){
	for(j=0;j<width;j++){
		hist_gradient[gradient_Map[i][j]]++;
		}
	}

for (i=0;i<(max_gradient+1);i++){
	printf("%u -> %u\n", i, hist_gradient[i]);
}

return TRUE;
}

