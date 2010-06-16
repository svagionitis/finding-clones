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
#include <string.h>
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

(*width_subimages) =  width / SHIFT;
(*height_subimages) = height / SHIFT;
unsigned int sub_hei = (*height_subimages);
unsigned int sub_wid = (*width_subimages);

printf("SHIFT= %d, Width= %d, Height= %d, Width_Sub= %d, Height_Sub= %d\n", SHIFT, width, height, sub_wid, sub_hei);

unsigned int swdM = (width / SHIFT) - 1;
unsigned int swmM = width % SHIFT;
unsigned int shdM = (height / SHIFT) - 1;
unsigned int shmM = height % SHIFT;


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

	printf("Allocated %d bytes for RGB+Greyscale subimages data.\n", (((swdM * M) + swmM) * ((shdM * M) + shmM) * 4 * sizeof(unsigned char)));
	}/*else subimage_data*/


/* Populate subimage data*/
for(i=0;i<sub_hei;i++){/*Height coordinate of subimage*/
	unsigned int ssi = SHIFT*i;
	for (j=0;j<sub_wid;j++){/*Width coordinate of subimage*/
		unsigned int ssj = SHIFT*j;
		
		/*------------------------------------------------*/
		if (i < shdM)
			h_mem_alloc = M;
		else
			h_mem_alloc = shmM;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){
			unsigned int x = (k + ssi);
			/*------------------------------------------------*/
			if (j < swdM)
				w_mem_alloc = M;
			else
				w_mem_alloc = swmM;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				unsigned int y = (l + ssj);

				unsigned int xyM = (y + x * (width)) * 3;

				subimage_data[i][j][k][l][0] = image_data[xyM + 0];
				subimage_data[i][j][k][l][1] = image_data[xyM + 1];
				subimage_data[i][j][k][l][2] = image_data[xyM + 2];

				r = subimage_data[i][j][k][l][0];
				g = subimage_data[i][j][k][l][1];
				b = subimage_data[i][j][k][l][2];
				subimage_data[i][j][k][l][3] = GREYSCALE1(r, g, b);

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
int export_ppm_subimages(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;
unsigned int data_mem_alloc = 0;
char filename[128];

unsigned int swdM = (width / SHIFT) - 1;
unsigned int swmM = width % SHIFT;
unsigned int shdM = (height / SHIFT) - 1;
unsigned int shmM = height % SHIFT;

unsigned char *data_buffer=NULL;
data_mem_alloc = (3 * M * M);
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
				}/*l*/
			}/*k*/
		sprintf(filename,"%03u_%03u_Subimage[%02ux%02u].ppm",i, j, h_mem_alloc, w_mem_alloc);
		save_ppm(filename, w_mem_alloc, h_mem_alloc, data_buffer);
		}/*j*/
	}/*i*/


free(data_buffer);

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
width_subimages(IN)   unsigned int        Width coordinate of subimage.
height_subimages(IN)  unsigned int        Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.
r, g, b               unsigned char       RGB values.

############################################################################ */
int calculate_histogram(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int pixel_value_counter[255];
memset(pixel_value_counter, 0, sizeof(pixel_value_counter));

unsigned int swdM = (width / SHIFT) - 1;
unsigned int swmM = width % SHIFT;
unsigned int shdM = (height / SHIFT) - 1;
unsigned int shmM = height % SHIFT;

float ***hist_data;

/* Allocate memory for subimage data*/
hist_data = (float ***)malloc(height_subimages * sizeof(float **));
if (hist_data == NULL){
	printf("Could not allocate %d bytes.\n", (height_subimages * sizeof(float **)));
	return FALSE;
	}
else{
	for (i=0;i<height_subimages;i++){
		hist_data[i] = (float **)malloc(width_subimages * sizeof(float *));
		if (hist_data[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", (width_subimages * sizeof(float *)), i);
			return FALSE;
			}
		else{
			for (j=0;j<width_subimages;j++){
				hist_data[i][j] = (float *)malloc(255 * sizeof(float));
				if (hist_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (255 * sizeof(float)), j);
					return FALSE;
					}
				else{
					for(k=0;k<255;k++){
						hist_data[i][j][k] = 0.0;
						}/*for k*/
					}/*else hist_data[i][j]*/
				}/*for j*/
			}/*else hist_data[i]*/
		}/*for i*/
	}/*else hist_data*/


unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;
for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*------------------------------------------------*/
		if (i < shdM)
			h_mem_alloc = M;
		else
			h_mem_alloc = shmM;
		/*------------------------------------------------*/
		
		memset(pixel_value_counter, 0, sizeof(pixel_value_counter));
		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < swdM)
				w_mem_alloc = M;
			else
				w_mem_alloc = swmM;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				unsigned char r = 0, g = 0, b = 0, gry = 0;
				switch(type){
					case 0:/*Red Histogram*/
						r = subimage_data[i][j][k][l][0];
						pixel_value_counter[r]++;
						break;
					case 1:/*Green Histogram*/
						g = subimage_data[i][j][k][l][1];
						pixel_value_counter[g]++;
						break;
					case 2:/*Blue Histogram*/
						b = subimage_data[i][j][k][l][2];
						pixel_value_counter[b]++;
						break;
					case 3:/*Grey Histogram*/
						gry = subimage_data[i][j][k][l][3];
						pixel_value_counter[gry]++;
						break;
					}
				}/*for l*/
			}/*for k*/

		for (m=0;m<255;m++){
			hist_data[i][j][m] = ((float)pixel_value_counter[m] / (h_mem_alloc*w_mem_alloc));
			if (hist_data[i][j][m])
				printf("[%d] [%u %u %u] %f\n", pixel_value_counter[m], i, j, m, hist_data[i][j][m]);
			}

		}/*for j*/
	}/*for i*/


return TRUE;
}


