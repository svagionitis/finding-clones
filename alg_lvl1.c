/* ############################################################################
Name           : alg_lvl1.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm Level 1.

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */

#define __ALG_LVL1_C__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "alg_lvl1.h"
#include "alcon2009.h"/*For save_ppm function*/

unsigned char *****subimage_data;

/* ############################################################################
Name           : create_sub_images
Description    : Take as input the original image and create the subimages 
                 according to the algorithm of the paper. The subimages are 
                 colored.

Arguments             Type                Description
===================== =================== =====================================
image_data(IN)        unsigned char *     Image data.
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
subimage_data(OUT)    unsigned char ***** Subimages data. 
                                          [Width-coodinate]
                                          [Height-Coordinate]
                                          [x-coordinate of subimage]
                                          [y-cooddinate of subimage]
                                          [RGB values + Greyscale value]
width_subimages(OUT)  unsigned int*       Width coordinate of subimage.
height_subimages(OUT) unsigned int*       Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
r, g, b               unsigned char       RGB values.
step_per_subimage     unsigned int        Shift step per subimage.

############################################################################ */
int create_sub_images(unsigned char *image_data, int width, int height, unsigned int *width_subimages, unsigned int *height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned char r = 0, g = 0, b = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;

(*width_subimages) =  (unsigned int)ceil((double)STEP * ((double)width / M));
(*height_subimages) = (unsigned int)ceil((double)STEP * ((double)height / M));
unsigned int sub_hei = (*height_subimages);
unsigned int sub_wid = (*width_subimages);

printf("shift= %d, step= %d, width= %d,  height= %d, width_sub= %d, height_sub= %d\n", SHIFT, STEP, width, height, sub_wid, sub_hei);

unsigned int swdM = STEP*width / M;
unsigned int swmM = STEP*width % M;
unsigned int shdM = STEP*height / M;
unsigned int shmM = STEP*height % M;
printf("Width Div: %d, Mod: %d\n", swdM, swmM);
printf("Height Div: %d, Mod: %d\n", shdM, shmM);


/* Allocate memory for subimage data*/
subimage_data = (unsigned char *****)malloc(sub_hei * sizeof(unsigned char ****));
if (subimage_data == NULL){
	printf("Could not allocate %d bytes.\n", (sub_hei * sizeof(unsigned char ****)));
	return FALSE;
	}
else{
	for (i=0;i<sub_hei;i++){
		subimage_data[i] = (unsigned char ****)malloc(sub_wid * sizeof(unsigned char ***));
		if (subimage_data[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", (sub_wid * sizeof(unsigned char ***)), i);
			return FALSE;
			}
		else{
			for (j=0;j<sub_wid;j++){

				/*------------------------------------------------*/
				if (i < shdM){
					h_mem_alloc = M;
					subimage_data[i][j] = (unsigned char ***)malloc(h_mem_alloc * sizeof(unsigned char **));
					}
				else{
					h_mem_alloc = shmM;
					subimage_data[i][j] = (unsigned char ***)malloc(h_mem_alloc * sizeof(unsigned char **));
					}
				/*------------------------------------------------*/

				if (subimage_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (h_mem_alloc * sizeof(unsigned char **)), j);
					return FALSE;
					}
				else{
					for(k=0;k<h_mem_alloc;k++){

						/*------------------------------------------------*/
						if (j < swdM){
							w_mem_alloc = M;
							subimage_data[i][j][k] = (unsigned char **)malloc(w_mem_alloc * sizeof(unsigned char *));
							}
						else{
							w_mem_alloc = swmM;
							subimage_data[i][j][k] = (unsigned char **)malloc(w_mem_alloc * sizeof(unsigned char *));
							}
						/*------------------------------------------------*/

						if (subimage_data[i][j][k] == NULL){
							printf("Could not allocate %d bytes for k=%d index.\n", (w_mem_alloc * sizeof(unsigned char)), k);
							return FALSE;
							}
						else{
							for(l=0;l<w_mem_alloc;l++){
								subimage_data[i][j][k][l] = (unsigned char *)malloc(4 * sizeof(unsigned char));
								if (subimage_data[i][j][k][l] == NULL){
									printf("Could not allocate %d bytes for l=%d index.\n", (4 * sizeof(unsigned char)), l);
									return FALSE;
									}
								else{
									/*Initialize subimage data.*/
									for(m=0;m<4;m++){
										subimage_data[i][j][k][l][m] = 0;
										}
									}
								}/*for l*/
							}/*else subimage_data[i][j][k]*/
						}/*for k*/
					}/*else subimage_data[i][j]*/
				}/*for j*/
			}/*else subimage_data[i]*/
		}/*for i*/

	printf("Allocated %d bytes for RGB+Greyscale subimages data.\n", ((swdM * M + swmM) * (shdM * M + shmM) * 4 * sizeof(unsigned char)));
	}/*else subimage_data*/

unsigned int x = 0, y = 0;
unsigned int ssi = 0, ssj = 0, xyM = 0;
unsigned int ri = 0, gi = 0, bi = 0;

/* Populate subimage data*/
for(i=0;i<sub_hei;i++){/*Height coordinate of subimage*/
	ssi = SHIFT*i;
	for (j=0;j<sub_wid;j++){/*Width coordinate of subimage*/
		ssj = SHIFT*j;
		
		if (i == 1) return TRUE;

		/*------------------------------------------------*/
		if (i < shdM)
			h_mem_alloc = M;
		else
			h_mem_alloc = shmM;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){
			x = (k + ssi);
			/*------------------------------------------------*/
			if (j < swdM)
				w_mem_alloc = M;
			else
				w_mem_alloc = swmM;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				/*y = ((l + ssj)+((k + i*M)*(width - M)));*/
				/*y = ((l + ssj)+((k + ssi)*(width - w_mem_alloc)));*/

				xyM = ((l + ssj) + (k + ssi) * (width)) * 3;
				if ((k == 0 && l == 0) || ((k == h_mem_alloc - 1) && (l == w_mem_alloc - 1)))
					printf("[%d %d %d %d] - [%d %d] [%d]\n", i, j, k, l, (k + ssi), (l + ssj), xyM);
				//printf("[%d %d %d %d] - [%d %d] [%d]\n", i, j, k, l, (l + ssj), (k + ssi), xyM);
				/*xyM = (y + x*M)*3;*/
				/*xyM = (y + x*w_mem_alloc)*3;*/


				if (xyM > (width*height*3))
					continue;

				ri = xyM + 0;
				gi = xyM + 1;
				bi = xyM + 2;
				subimage_data[i][j][k][l][0] = image_data[ri];
				subimage_data[i][j][k][l][1] = image_data[gi];
				subimage_data[i][j][k][l][2] = image_data[bi];

				r = subimage_data[i][j][k][l][0];
				g = subimage_data[i][j][k][l][1];
				b = subimage_data[i][j][k][l][2];
				subimage_data[i][j][k][l][3] = GREYSCALE1(r, g, b);
				//printf("[%d %d %d %d] - [%d %d] [%d]\n", i, j, k, l, x, y, ri);

				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/


return TRUE;
}


/* ############################################################################
Name           : export_ppm_subimages_rgb
Description    : Take as input the original image and create the subimages 
                 according to the algorithm of the paper. The subimages are 
                 colored.

Arguments             Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data. 
                                          [Width-coodinate]
                                          [Height-Coordinate]
                                          [x-coordinate of subimage]
                                          [y-cooddinate of subimage]
                                          [RGB values + Greyscale value]
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
width_subimages(IN)   unsigned int*       Width coordinate of subimage.
height_subimages(IN)  unsigned int*       Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
r, g, b               unsigned char       RGB values.

############################################################################ */
int export_ppm_subimages_rgb(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;
unsigned int data_mem_alloc = 0;
char filename[128];

unsigned int swdM = STEP*width / M;
unsigned int swmM = STEP*width % M;
unsigned int shdM = STEP*height / M;
unsigned int shmM = STEP*height % M;

unsigned char *data_buffer=NULL;
data_mem_alloc = (3 * M * M);
//alloc_mem_1D_array(data_buffer, data_mem_alloc);
data_buffer = (unsigned char *)malloc(data_mem_alloc * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("Could not allocate %d bytes.\n", (data_mem_alloc * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	printf("Mem alloc %d bytes.\n", data_mem_alloc * sizeof(unsigned char));
	for(i=0;i<data_mem_alloc;i++)
		data_buffer[i] = 0;
	}


for(i=0;i<height_subimages;i++){/*Height coordinate of subimage*/
	for (j=0;j<width_subimages;j++){/*Width coordinate of subimage*/

		if (i == 1) return TRUE;

		/*------------------------------------------------*/
		if (i < shdM)
			h_mem_alloc = M;
		else
			h_mem_alloc = shmM;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < swdM)
				w_mem_alloc = M;
			else
				w_mem_alloc = swmM;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				unsigned int lkwma = (l + k*w_mem_alloc)*3;
				data_buffer[lkwma + 0] = subimage_data[i][j][k][l][0];
				data_buffer[lkwma + 1] = subimage_data[i][j][k][l][1];
				data_buffer[lkwma + 2] = subimage_data[i][j][k][l][2];
				//printf("[%d %d, %d %d, %d] %d\n", i, j, k, l, m, ((l + k*M)*3 + m));
				}/*l*/
			}/*k*/
		sprintf(filename,"%03u_%03u_Subimage[%dx%d].ppm",i, j, h_mem_alloc, w_mem_alloc);
		save_ppm(filename, w_mem_alloc, h_mem_alloc, data_buffer);
		}/*j*/
	}/*i*/


free(data_buffer);

return TRUE;
}



/*
int calculate_histogram(unsigned char type)
{
}
*/

