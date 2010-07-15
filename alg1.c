/* ############################################################################
Name           : alg1.c
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Functions for Algorithm 1. Implementation of the algorithm 
                 described in 
                 Jiang J.-A., Chang H.-Y., Wu K.-H., Ouyang C.-S., Yang M.-M., 
                 Yang E.-C., Chen T.-W., Lin T.-T. An adaptive image 
                 segmentation algorithm for X-ray quarantine inspection of
                 selected fruits (2008) Computers and Electronics in Agriculture, 
                 60 (2), pp. 190-200.
Procedure                                 Description
========================================= =====================================
create_sub_images
free_mem_subimages
export_ppm_subimages

calculate_histogram
free_mem_histogram

calculate_threshold
calculate_min_max_hist_threshold
basic_global_thresholding_algorithm

calculate_threshold_with_interpolation
bilinear_interpolation_with_weights_...
propotional_to_square_of_distance
bilinear_interpolation_with_weights_...
propotional_to_distance
linear_interpolation_in_2d_data


Globals               Type                Description
===================== =================== =====================================
subimage_data         unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]
hist_data             histogram ***       Histogram from subimages.
Ts                    unsigned char **    Calculate one threshold per subimage.

Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */

#define __ALG1_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>/*UINT_MAX*/

#include "alg1.h"
#include "alcon2009.h"/*For save_ppm function*/
#include "morphological_operations.h"

unsigned char *****subimage_data;
histogram ***hist_data;
unsigned char **Ts;



int Calculate_Block_Size(unsigned int HeightBlockIndex, unsigned int WidthBlockIndex, int height, int width, unsigned int *HeightBlockSize, unsigned int *WidthBlockSize)
{
unsigned int HeightBlockStart = HeightBlockIndex * SHIFT;
unsigned int WidthBlockStart = WidthBlockIndex * SHIFT;

if ((HeightBlockStart + M) <= height)
	(*HeightBlockSize) = M;
else
	(*HeightBlockSize) = height - HeightBlockStart;

if ((WidthBlockStart + M) <= width)
	(*WidthBlockSize) = M;
else
	(*WidthBlockSize) = width - WidthBlockStart;

return TRUE;
}

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
width_subimages(OUT)  unsigned int*       Width coordinate of subimage.
height_subimages(OUT) unsigned int*       Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.
FALSE                                     If memory allocation fails.

Globals               Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
r, g, b               unsigned char       RGB values.
h_mem_alloc           unsigned int        The maximum height of the subimage.
w_mem_alloc           unsigned int        The maximum width of the subimage.

############################################################################ */
int create_sub_images(unsigned char *image_data, int width, int height, unsigned int *width_subimages, unsigned int *height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;
unsigned int SdM = (SHIFT / M);

if (WIDTH_MOD_SHIFT(width, SHIFT))
	(*width_subimages) =  WIDTH_DIV_SHIFT(width, SHIFT) + SdM;
else
	(*width_subimages) =  WIDTH_DIV_SHIFT(width, SHIFT);

if (HEIGHT_MOD_SHIFT(height, SHIFT))
	(*height_subimages) = HEIGHT_DIV_SHIFT(height, SHIFT) + SdM;
else
	(*height_subimages) = HEIGHT_DIV_SHIFT(height, SHIFT);

unsigned int sub_hei = (*height_subimages);
unsigned int sub_wid = (*width_subimages);

#if ALLOW_PRINTF == TRUE
printf("SHIFT= %d, Width= %d, Height= %d, Width_Sub= %d, Height_Sub= %d Width_Mod= %d Height_Mod= %d\n", SHIFT, width, height, sub_wid, sub_hei, WIDTH_MOD_SHIFT(width, SHIFT), HEIGHT_MOD_SHIFT(height, SHIFT));
#endif

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
				Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
				/*------------------------------------------------*/

				subimage_data[i][j] = (unsigned char ***)malloc(HeightBlockSize * sizeof(unsigned char **));
				if (subimage_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (HeightBlockSize * sizeof(unsigned char **)), j);
					return FALSE;
					}
				else{
					for(k=0;k<HeightBlockSize;k++){
						subimage_data[i][j][k] = (unsigned char **)malloc(WidthBlockSize * sizeof(unsigned char *));
						if (subimage_data[i][j][k] == NULL){
							printf("Could not allocate %d bytes for k=%d index.\n", (WidthBlockSize * sizeof(unsigned char)), k);
							return FALSE;
							}
						else{
							for(l=0;l<WidthBlockSize;l++){
								subimage_data[i][j][k][l] = (unsigned char *)malloc(5 * sizeof(unsigned char));
								if (subimage_data[i][j][k][l] == NULL){
									printf("Could not allocate %d bytes for l=%d index.\n", (5 * sizeof(unsigned char)), l);
									return FALSE;
									}
								else{
									/*Initialize subimage data.*/
									for(m=0;m<5;m++){
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
	}/*else subimage_data*/

/* Populate subimage data*/
for(i=0;i<sub_hei;i++){/*Height coordinate of subimage*/
	unsigned int HeightBlockStart = i * SHIFT;
	for (j=0;j<sub_wid;j++){/*Width coordinate of subimage*/
		unsigned int WidthBlockStart = j * SHIFT;
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			unsigned int x = (k + HeightBlockStart);
			for(l=0;l<WidthBlockSize;l++){
				unsigned char r = 0, g = 0, b = 0;
				unsigned int y = (l + WidthBlockStart);

				register unsigned int xyM = (y + x * (width)) * 3;

				r = subimage_data[i][j][k][l][0] = image_data[xyM + 0];
				g = subimage_data[i][j][k][l][1] = image_data[xyM + 1];
				b = subimage_data[i][j][k][l][2] = image_data[xyM + 2];

				subimage_data[i][j][k][l][3] = GREYSCALE1(r, g, b);
				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/

return TRUE;
}


/* ############################################################################
Name           : free_mem_subimages
Description    : Free memory from subimages.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
width_subimages(IN)   unsigned int        Width coordinate of subimage.
height_subimages(IN)  unsigned int        Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.


Globals               Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
HeightBlockSize           unsigned int        The maximum height of the subimage.
WidthBlockSize           unsigned int        The maximum width of the subimage.

############################################################################ */
int free_mem_subimages(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;


for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			for(l=0;l<WidthBlockSize;l++){
				free(subimage_data[i][j][k][l]);
				}
			free(subimage_data[i][j][k]);
			}
		free(subimage_data[i][j]);
		}
	free(subimage_data[i]);
	}

free(subimage_data);

return TRUE;
}

/* ############################################################################
Name           : export_ppm_subimages
Description    : Take as input the original image and create the subimages 
                 according to the algorithm of the paper. The subimages are 
                 colored.

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       0 for red value pixels
                                          1 for green value pixels
                                          2 for blue value pixels
                                          3 for grey value pixels
                                          4 for RGB value pixels
                                          5 for threshold values
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
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
HeightBlockSize           unsigned int        The maximum height of the subimage.
WidthBlockSize           unsigned int        The maximum width of the subimage.

############################################################################ */
int export_ppm_subimages(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;

unsigned int data_mem_alloc = 0;
char filename[128];
memset(filename, '\0', sizeof(filename));


unsigned char *data_buffer=NULL;
data_mem_alloc = (3 * M * M);
data_buffer = (unsigned char *)malloc(data_mem_alloc * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("Could not allocate %d bytes.\n", (data_mem_alloc * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	for(i=0;i<data_mem_alloc;i++)
		data_buffer[i] = 0;
	}


for(i=0;i<height_subimages;i++){/*Height coordinate of subimage*/
	for (j=0;j<width_subimages;j++){/*Width coordinate of subimage*/
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			for(l=0;l<WidthBlockSize;l++){
				register unsigned int lkwma = (l + k*WidthBlockSize)*3;
				switch(type){
					case 0:/*Red values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][0];
						data_buffer[lkwma + 1] = 0;
						data_buffer[lkwma + 2] = 0;
						sprintf(filename,"%s_%03u_%03u_RedSubimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					case 1:/*Green values*/
						data_buffer[lkwma + 0] = 0;
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][1];
						data_buffer[lkwma + 2] = 0;
						sprintf(filename,"%s_%03u_%03u_GreenSubimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					case 2:/*Blue values*/
						data_buffer[lkwma + 0] = 0;
						data_buffer[lkwma + 1] = 0;
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][2];
						sprintf(filename,"%s_%03u_%03u_BlueSubimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					case 3:/*Grey values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][3];
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][3];
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][3];
						sprintf(filename,"%s_%03u_%03u_GreySubimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					case 4:/*RGB values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][0];
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][1];
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][2];
						sprintf(filename,"%s_%03u_%03u_Subimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					case 5:/*Threshold values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][4];
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][4];
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][4];
						sprintf(filename,"%s_%03u_%03u_ThresholdSubimage[%02ux%02u].ppm",output_fn, i, j, HeightBlockSize, WidthBlockSize);
						break;
					}
				}/*l*/
			}/*k*/
		save_ppm(filename, WidthBlockSize, HeightBlockSize, data_buffer);
		}/*j*/
	}/*i*/


free(data_buffer);

return TRUE;
}


/* ############################################################################
Name           : calculate_histogram
Description    : Calculate the histogram for each subimage.

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       0 for red value pixels
                                          1 for green value pixels
                                          2 for blue value pixels
                                          3 for grey value pixels
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
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

hist_data(OUT)        histogram ***       Histogram from subimages.

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.
HeightBlockSize           unsigned int        The maximum height of the subimage.
WidthBlockSize           unsigned int        The maximum width of the subimage.
r, g, b               unsigned char       RGB values.
gry                   unsigned char       Grey value.

############################################################################ */
int calculate_histogram(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;

unsigned int pixel_value_counter[COLORS], max_pixels = 0;
memset(pixel_value_counter, 0, sizeof(pixel_value_counter));


/* Allocate memory for histogram data*/
hist_data = (histogram ***)malloc(height_subimages * sizeof(histogram **));
if (hist_data == NULL){
	printf("Could not allocate %d bytes.\n", (height_subimages * sizeof(histogram **)));
	return FALSE;
	}
else{
	for (i=0;i<height_subimages;i++){
		hist_data[i] = (histogram **)malloc(width_subimages * sizeof(histogram *));
		if (hist_data[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", (width_subimages * sizeof(histogram *)), i);
			return FALSE;
			}
		else{
			for (j=0;j<width_subimages;j++){
				hist_data[i][j] = (histogram *)malloc(COLORS * sizeof(histogram));
				if (hist_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (COLORS * sizeof(histogram)), j);
					return FALSE;
					}
				else{
					for(k=0;k<COLORS;k++){
						hist_data[i][j][k].num_pixels = 0;
						hist_data[i][j][k].freq = 0.0;
						}/*for k*/
					}/*else hist_data[i][j]*/
				}/*for j*/
			}/*else hist_data[i]*/
		}/*for i*/
	}/*else hist_data*/


for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/
		
		memset(pixel_value_counter, 0, sizeof(pixel_value_counter));
		for(k=0;k<HeightBlockSize;k++){
			max_pixels = (HeightBlockSize*WidthBlockSize);
			for(l=0;l<WidthBlockSize;l++){
				switch(type){
					unsigned char r = 0, g = 0, b = 0, gry = 0;
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
					}/*switch*/
				}/*for l*/
			}/*for k*/

		for (m=0;m<COLORS;m++){
			hist_data[i][j][m].num_pixels = pixel_value_counter[m];
			hist_data[i][j][m].freq = ((float)pixel_value_counter[m] / max_pixels);
			}/*for m*/

		}/*for j*/
	}/*for i*/

return TRUE;
}


/* ############################################################################
Name           : free_mem_histogram
Description    : Free memory from histogram.

Arguments             Type                Description
===================== =================== =====================================
width_subimages(IN)   unsigned int        Width coordinate of subimage.
height_subimages(IN)  unsigned int        Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.


Globals               Type                Description
===================== =================== =====================================
hist_data(IN)         histogram ***       Histogram from subimages.

Locals                Type                Description
===================== =================== =====================================
i, j                  unsigned int        General purpose indexes.

############################################################################ */
int free_mem_histogram(unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0;

for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		free(hist_data[i][j]);
		}/*for j*/
	free(hist_data[i]);
	}/*for i*/

free(hist_data);

return TRUE;
}



/* ############################################################################
Name           : calculate_threshold
Description    : Calculate threshold values according to the algorithm 
                 from Gonzales, R.C, Woods, R.E., 2002. Digital Image 
                 ProceHeightBlockStartng, 2nd ed Prentice Hall, Upper Saddle River, NJ, 
                 pp. 598-600(10.3.3 Basic Global Thresholding).

Arguments             Type                Description
===================== =================== =====================================
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
hist_data(IN)         histogram ***       Histogram from subimages.
Ts(OUT)               unsigned char **    Calculate one threshold per subimage.

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.

############################################################################ */
int calculate_threshold(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0;

/* Allocate memory for threshold data*/
Ts = (unsigned char **)malloc(height_subimages * sizeof(unsigned char *));
if (Ts == NULL){
	printf("Could not allocate %d bytes.\n", (height_subimages * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height_subimages;i++){
		Ts[i] = (unsigned char *)malloc(width_subimages * sizeof(unsigned char));
		if (Ts[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", (width_subimages * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for (j=0;j<width_subimages;j++){
				Ts[i][j] = 0;
				}/*for j*/
			}
		}/*for i*/
	}

/* initialize random seed: */
srand (time(NULL));

for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*********************Begin Step 1*********************/
		/*Find min and max value of histogram*/
		unsigned char max_gry_lvl = 0, min_gry_lvl = 0;
		calculate_min_max_hist_threshold(i, j, &max_gry_lvl, &min_gry_lvl);

		/*printf("[%d %d] [max_level:%d min_level:%d]\n", i, j, max_gry_lvl, min_gry_lvl);*/

		/*Initial value of T is the mean value of min and max of grey levels*/
		float Tstart = ((float)(max_gry_lvl + min_gry_lvl) / 2);

		/*Initial value of T is a random value between 0 and COLORS-1*/
		/*float Tstart = rand()/(float)(((float)RAND_MAX + 1) / COLORS);*/

		/*Initial value of T is a 0*/
		/*float Tstart = 0.0;*/

		/*printf("Tstart:%f\n", Tstart);*/
		/*********************End Step 1*********************/

		basic_global_thresholding_algorithm(i, j, Tstart, &Ts[i][j]);

		}/*for j*/
	}/*for i*/

return TRUE;
}


/* ############################################################################
Name           : calculate_min_max_hist_threshold
Description    : Calculate the minimum and maximum grey 
                 level value of the histogram.

Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned int        Index for height coordinate of 
                                          subimage.
w_subim_index(IN)     unsigned int        Index for width coordinate of 
                                          subimage.
max_lvl_hist(OUT)     unsigned char *     The level of histogram that has the 
                                          maximum value.
min_lvl_hist(OUT)     unsigned char *     The level of histogram that has the 
                                          minimum value.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
hist_data             histogram ***       The histogram of the subimages.

Locals                Type                Description
===================== =================== =====================================
i                     unsigned int        General purpose index.
max_pix_hist          unsigned int        The maximum value of histogram.
min_pix_hist          unsigned int        The minimum value of histogram
max_lvl               unsigned char       The level of histogram that has the 
                                          maximum value.
min_lvl               unsigned char       The level of histogram that has the 
                                          minimum value.

############################################################################ */
int calculate_min_max_hist_threshold(unsigned int h_subim_index, unsigned int w_subim_index, unsigned char *max_lvl_hist, unsigned char *min_lvl_hist)
{
unsigned int i = 0;
/*Number of pixels for each level*/
unsigned int max_pix_hist = 0, min_pix_hist = UINT_MAX;
unsigned char max_lvl = 0, min_lvl = 0;

for (i=0;i<COLORS;i++){

	if (hist_data[h_subim_index][w_subim_index][i].num_pixels > max_pix_hist){
		max_pix_hist = hist_data[h_subim_index][w_subim_index][i].num_pixels;
		max_lvl = i;
		}

	if (hist_data[h_subim_index][w_subim_index][i].num_pixels <= min_pix_hist){
		min_pix_hist = hist_data[h_subim_index][w_subim_index][i].num_pixels;
		min_lvl = i;
		}
	}

(*max_lvl_hist) = max_lvl;
(*min_lvl_hist) = min_lvl;

return TRUE;
}


/* ############################################################################
Name           : basic_global_thresholding_algorithm
Description    : Calculate threshold values according to the algorithm 
                 from Gonzales, R.C, Woods, R.E., 2002. Digital Image 
                 ProceHeightBlockStartng, 2nd ed Prentice Hall, Upper Saddle River, NJ, 
                 pp. 598-600(10.3.3 Basic Global Thresholding).


Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned int        Index for height coordinate of 
                                          subimage.
w_subim_index(IN)     unsigned int        Index for width coordinate of 
                                          subimage.
Tstart(IN)            float               The initial value for threshold.
Ts(OUT)               unsigned char *     The output value for threshold.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
hist_data             histogram ***       The histogram of the subimages.

Locals                Type                Description
===================== =================== =====================================
i                     unsigned int        General purpose index.
Tend                  float               The output value of threshold.
first_run_flag        unsigned char       A flag for getting into the loop for 
                                          first time.
mi1_values            unsigned int        Sum of pixel values in G1.
mi2_values            unsigned int        Sum of pixel values in G2.
count_mi1             unsigned int        Total number of pixels that are 
                                          located in G1 region.
count_mi2             unsigned int        Total number of pixels that are 
                                          located in G2 region.
mi1                   float               Average grey level value for 
                                          region G1.
mi2                   float               Average grey level value for 
                                          region G2.

############################################################################ */
int basic_global_thresholding_algorithm(unsigned int h_subim_index, unsigned int w_subim_index, float Tstart, unsigned char *Ts)
{
unsigned int i = 0;
float Tend = 0.0;
unsigned char first_run_flag = TRUE;
while(fabs(Tstart - Tend) > DIFF_T){/*Step 5*/
	if (!first_run_flag)
		memcpy(&Tstart, &Tend, sizeof(Tend));

	/*printf("Tstart:%f Tend:%f\n", Tstart, Tend);*/
	/*********************Begin Step 2 & 3*********************/
	/*
	* Calculate the average grey level values mi1 and mi2 for the pixels 
	* in regions G1 and G2.
	*/
	unsigned int mi1_values = 0, mi2_values = 0;
	unsigned int count_mi1 = 0, count_mi2 = 0;
	float mi1 = 0, mi2 = 0;
	for (i=0;i<COLORS;i++){
		if (i > (unsigned int)Tstart){/*G1 region*/
			mi1_values += (i*hist_data[h_subim_index][w_subim_index][i].num_pixels);
			count_mi1 += hist_data[h_subim_index][w_subim_index][i].num_pixels;
			}
		else if (i <= (unsigned int)Tstart){/*G2 region*/
			mi2_values += (i*hist_data[h_subim_index][w_subim_index][i].num_pixels);
			count_mi2 += hist_data[h_subim_index][w_subim_index][i].num_pixels;
			}
		}

	if (!count_mi1)/*if count_mi1 == 0*/
		count_mi1 = 1;
	if (!count_mi2)/*if count_mi2 == 0*/
		count_mi2 = 1;

	mi1 = ((float)mi1_values / count_mi1);
	mi2 = ((float)mi2_values / count_mi2);
	/*printf("[mi1:%f mi2:%f][mi1_values:%u mi2_values:%u] [count_mi1:%u count_mi2:%u]\n", mi1, mi2, mi1_values, mi2_values, count_mi1, count_mi2);*/
	/*********************End Step 2 & 3*********************/

	/*********************Begin Step 4*********************/
	Tend = ((float)(mi1 + mi2) / 2);
	/*printf("Tend:%f\n", Tend);*/
	/*********************End Step 4*********************/
	first_run_flag = FALSE;
	}/*while Step 5*/

/*printf("[%u %u] Tend:%f\n", h_subim_index, w_subim_index, Tend);*/
(*Ts) = (unsigned char)Tend;

return TRUE;
}



/* ############################################################################
Name           : calculate_threshold_with_interpolation
Description    : Calculate thresholds using interpolation.

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       0 bilinear interpolation with weights 
                                            propotional to square of distance.
                                          1 bilinear interpolation with weights 
                                            propotional to distance.
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
width_subimages(IN)   unsigned int        Width coordinate of subimage.
height_subimages(IN)  unsigned int        Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
subimage_data(OUT)    unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.
HeightBlockSize           unsigned int        The maximum height of the subimage.
WidthBlockSize           unsigned int        The maximum width of the subimage.

############################################################################ */
int calculate_threshold_with_interpolation(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;


/*
 * AHeightBlockStartgn in (0,0) position in a subimage the 
 * Threshold value that it was calculated before. 
 * The others values will be calculated using interpolation.
 */
for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			for(l=0;l<WidthBlockSize;l++){
				unsigned int TsHeightPos = 0;
				unsigned int TsWidthPos = 0;
				subimage_data[i][j][TsHeightPos][TsWidthPos][4] = Ts[i][j];
				}/*for l*/
			}/*for k*/
		}/*for j*/
	}/*for i*/

for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			for(l=0;l<WidthBlockSize;l++){
				unsigned int TsHeightPos = 0;
				unsigned int TsWidthPos = 0;

				if (!k && !l)/*Don't compute the position (0,0)*/
					continue;

				switch(type){
					case 0:
						if ((i < (height_subimages - 1)) && (j < (width_subimages - 1))){
							bilinear_interpolation_with_weights_propotional_to_square_of_distance(i, j, k, l, HeightBlockSize, WidthBlockSize, TsHeightPos, TsWidthPos, &subimage_data[i][j][k][l][4]);
							}
						else{
							linear_interpolation_in_2d_data(i, j, height_subimages, width_subimages, k, l, HeightBlockSize, WidthBlockSize, TsHeightPos, TsWidthPos, &subimage_data[i][j][k][l][4]);
							}
						break;
					case 1:
						if ((i < (height_subimages - 1)) && (j < (width_subimages - 1))){
							bilinear_interpolation_with_weights_propotional_to_distance(i, j, k, l, HeightBlockSize, WidthBlockSize, TsHeightPos, TsWidthPos, &subimage_data[i][j][k][l][4]);
							}
						else{
							linear_interpolation_in_2d_data(i, j, height_subimages, width_subimages, k, l, HeightBlockSize, WidthBlockSize, TsHeightPos, TsWidthPos, &subimage_data[i][j][k][l][4]);
							}
						break;
					}
				}/*for l*/
			}/*for k*/
		}/*for j*/
	}/*for i*/


return TRUE;
}


/* ############################################################################
Name           : bilinear_interpolation_with_weights_propotional_to_square_of_distance
Description    : Bilinear interpolation with weights propotional to square of distance

Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned int        Index of subimage in height 
                                          coordinate.
w_subim_index(IN)     unsigned int        Index of subimage in width 
                                          coordinate.
x(IN)                 unsigned int        Height coordinate in the subimage.
y(IN)                 unsigned int        width coordinate in the subimage.
Max_x(IN)             unsigned int        Height of the subimage.
Max_y(IN)             unsigned int        Width of the subimage.
Ti(OUT)               unsigned char *     The interpolated threshold.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.

############################################################################ */
int bilinear_interpolation_with_weights_propotional_to_square_of_distance(unsigned int h_subim_index, unsigned int w_subim_index, unsigned int x, unsigned int y, unsigned int Max_x, unsigned int Max_y, unsigned int TsHeightPos, unsigned int TsWidthPos, unsigned char *Ti)
{
double I1 = 0.0, I2 = 0.0, I3 = 0.0, I4 = 0.0;
double R1POW2 = 0.0, R2POW2 = 0.0, R3POW2 = 0.0, R4POW2 = 0.0;
double OneDivR1POW2 = 0.0, OneDivR2POW2 = 0.0, OneDivR3POW2 = 0.0, OneDivR4POW2 = 0.0;
double I1DivR1POW2 = 0.0, I2DivR1POW2 = 0.0, I3DivR1POW2 = 0.0, I4DivR1POW2 = 0.0;
double SumODR = 0.0, SumIDR = 0.0;

I1 = (double)subimage_data[h_subim_index    ][w_subim_index    ][TsHeightPos][TsWidthPos][4];
I2 = (double)subimage_data[h_subim_index    ][w_subim_index + 1][TsHeightPos][TsWidthPos][4];
I3 = (double)subimage_data[h_subim_index + 1][w_subim_index + 1][TsHeightPos][TsWidthPos][4];
I4 = (double)subimage_data[h_subim_index + 1][w_subim_index    ][TsHeightPos][TsWidthPos][4];

/*
R1POW2 = (double)((x - TsHeightPos)	     *(x - TsHeightPos) 	  + (y - TsWidthPos)	  	      *(y - TsWidthPos));
R2POW2 = (double)((x - TsHeightPos)	     *(x - TsHeightPos) 	  + (y - (TsWidthPos + Max_y))	      *(y - (TsWidthPos + Max_y)));
R3POW2 = (double)((x - (TsHeightPos + Max_x))*(x - (TsHeightPos + Max_x)) + (y - (TsWidthPos + Max_y))	      *(y - (TsWidthPos + Max_y)));
R4POW2 = (double)((x - (TsHeightPos + Max_x))*(x - (TsHeightPos + Max_x)) + (y - TsWidthPos)	      	      *(y - TsWidthPos));
*/

R1POW2 = (double)(abs(x - TsHeightPos)		 + abs(y - TsWidthPos));
R2POW2 = (double)(abs(x - TsHeightPos)		 + abs(y - (TsWidthPos + Max_y)));
R3POW2 = (double)(abs(x - (TsHeightPos + Max_x)) + abs(y - (TsWidthPos + Max_y)));
R4POW2 = (double)(abs(x - (TsHeightPos + Max_x)) + abs(y - TsWidthPos));


OneDivR1POW2 = 1.0 / R1POW2;
OneDivR2POW2 = 1.0 / R2POW2;
OneDivR3POW2 = 1.0 / R3POW2;
OneDivR4POW2 = 1.0 / R4POW2;
SumODR = OneDivR1POW2 + OneDivR2POW2 + OneDivR3POW2 + OneDivR4POW2;

I1DivR1POW2 = I1 / R1POW2;
I2DivR1POW2 = I2 / R2POW2;
I3DivR1POW2 = I3 / R3POW2;
I4DivR1POW2 = I4 / R4POW2;
SumIDR = I1DivR1POW2 + I2DivR1POW2 + I3DivR1POW2 + I4DivR1POW2;

(*Ti) = (unsigned char)(SumIDR / SumODR);

return TRUE;
}


/* ############################################################################
Name           : bilinear_interpolation_with_weights_propotional_to_distance
Description    : Bilinear interpolation with weights propotional to distance.

Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned int        Index of subimage in height 
                                          coordinate.
w_subim_index(IN)     unsigned int        Index of subimage in width 
                                          coordinate.
x(IN)                 unsigned int        Height coordinate in the subimage.
y(IN)                 unsigned int        width coordinate in the subimage.
Max_x(IN)             unsigned int        Height of the subimage.
Max_y(IN)             unsigned int        Width of the subimage.
Ti(OUT)               unsigned char *     The interpolated threshold.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.

############################################################################ */
int bilinear_interpolation_with_weights_propotional_to_distance(unsigned int h_subim_index, unsigned int w_subim_index, unsigned int x, unsigned int y, unsigned int Max_x, unsigned int Max_y, unsigned int TsHeightPos, unsigned int TsWidthPos, unsigned char *Ti)
{
double I1 = 0.0, I2 = 0.0, I3 = 0.0, I4 = 0.0;
double Alpha = 0.0, Beta = 0.0, OneMinusAlpha = 0.0, OneMinusBeta = 0.0;

I1 = (double)subimage_data[h_subim_index    ][w_subim_index    ][TsHeightPos][TsWidthPos][4];
I2 = (double)subimage_data[h_subim_index    ][w_subim_index + 1][TsHeightPos][TsWidthPos][4];
I3 = (double)subimage_data[h_subim_index + 1][w_subim_index + 1][TsHeightPos][TsWidthPos][4];
I4 = (double)subimage_data[h_subim_index + 1][w_subim_index    ][TsHeightPos][TsWidthPos][4];

Alpha = ((double)(Max_x - x) / Max_x);
Beta =  ((double)(Max_y - y) / Max_y);
OneMinusAlpha = 1.0 - Alpha;
OneMinusBeta =  1.0 - Beta;

(*Ti) = (unsigned char)(((Alpha*Beta*I1)) + 
		        ((OneMinusAlpha*Beta*I2)) + 
		        ((Alpha*OneMinusBeta*I4)) + 
		        ((OneMinusAlpha*OneMinusBeta*I3)));

return TRUE;
}


/* ############################################################################
Name           : linear_interpolation_in_2d_data
Description    : Linear interpolation in 2-D data.

Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned int        Index of subimage in height 
                                          coordinate.
w_subim_index(IN)     unsigned int        Index of subimage in width 
                                          coordinate.
Max_Height_Subim(IN)  unsigned int        Number of subimages in the height of 
                                          the image.
Max_Width_Subim(IN)   unsigned int        Number of subimages in the width of 
                                          the image.
x(IN)                 unsigned int        Height coordinate in the subimage.
y(IN)                 unsigned int        width coordinate in the subimage.
Max_x(IN)             unsigned int        Height of the subimage.
Max_y(IN)             unsigned int        Width of the subimage.
Ti(OUT)               unsigned char *     The interpolated threshold.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.

Globals               Type                Description
===================== =================== =====================================
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.

############################################################################ */
int linear_interpolation_in_2d_data(unsigned int h_subim_index, unsigned int w_subim_index, unsigned int Max_Height_Subim, unsigned int Max_Width_Subim, unsigned int x, unsigned int y, unsigned int Max_x, unsigned int Max_y, unsigned int TsHeightPos, unsigned int TsWidthPos, unsigned char *Ti)
{
double I1 = 0.0, I2 = 0.0, I4 = 0.0;
/*
double Alpha = 0.0, OneMinusAlpha = 0.0;
*/
I1 = (double)subimage_data[h_subim_index][w_subim_index][TsHeightPos][TsWidthPos][4];
/*
Alpha = ((double)(Max_x - x) / Max_x) / ((double)(Max_y - y) / Max_y);
Alpha = ((double)x / Max_x) / ((double)y / Max_y);
OneMinusAlpha = 1.0 - Alpha;
*/

if ((w_subim_index == (Max_Width_Subim - 1)) && 
    (h_subim_index != (Max_Height_Subim - 1))){/*2 reference points*/

	I4 = (double)subimage_data[h_subim_index + 1][w_subim_index][TsHeightPos][TsWidthPos][4];
/*
	(*Ti) = (unsigned char)((Alpha*I1) + 
				(OneMinusAlpha*I4));
*/
	(*Ti) = (unsigned char)((0.5*I1) + 
				(0.5*I4));


	}
else if((h_subim_index == (Max_Height_Subim - 1)) &&
	(w_subim_index != (Max_Width_Subim - 1))){/*2 reference points*/

	I2 = (double)subimage_data[h_subim_index][w_subim_index + 1][TsHeightPos][TsWidthPos][4];

/*
	(*Ti) = (unsigned char)((Alpha*I1) + 
				(OneMinusAlpha*I2));
*/
	(*Ti) = (unsigned char)((0.5*I1) + 
				(0.5*I2));

	}
else if ((h_subim_index == (Max_Height_Subim - 1)) &&
	 (w_subim_index == (Max_Width_Subim - 1))){/*1 reference point*/

	(*Ti) = (unsigned char)I1;

	}

return TRUE;
}



/* ############################################################################
Name           : reconstruct_image_from_subimages
Description    : Reconstruct the initial image from subimages

Arguments             Type                Description
===================== =================== =====================================
type(IN)              unsigned char       0 for red value pixels
                                          1 for green value pixels
                                          2 for blue value pixels
                                          3 for grey value pixels
                                          4 for RGB value pixels
                                          5 for threshold values
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
subimage_data(IN)     unsigned char ***** Subimages data.
                                          [Width-coodinate of subimage]
                                          [Height-Coordinate of subimage]
                                          [x-coordinate in subimage]
                                          [y-cooddinate in subimage]
                                          [0: Red from RGB value
                                           1: Green from RGB value
                                           2: Blue from RGB value
                                           3: Grey value
                                           4: Threshold value]

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.
HeightBlockSize           unsigned int        The maximum height of the subimage.
WidthBlockSize           unsigned int        The maximum width of the subimage.

############################################################################ */
int reconstruct_image_from_subimages(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;

unsigned int data_mem_alloc = 0;
char filename[128];
memset(filename, '\0', sizeof(filename));

unsigned char *data_buffer=NULL;
data_mem_alloc = (3 * width * height);
data_buffer = (unsigned char *)malloc(data_mem_alloc * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("Could not allocate %d bytes.\n", (data_mem_alloc * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	for(i=0;i<data_mem_alloc;i++)
		data_buffer[i] = 0;
	}


switch(type){
	case 0:/*Red values*/
		sprintf(filename,"%s_Reconstructed_RedImage.ppm", output_fn);
		break;
	case 1:/*Green values*/
		sprintf(filename,"%s_Reconstructed_GreenImage.ppm", output_fn);
		break;
	case 2:/*Blue values*/
		sprintf(filename,"%s_Reconstructed_BlueImage.ppm", output_fn);
		break;
	case 3:/*Grey values*/
		sprintf(filename,"%s_Reconstructed_GreyImage.ppm", output_fn);
		break;
	case 4:/*RGB values*/
		sprintf(filename,"%s_Reconstructed_RGBImage.ppm", output_fn);
		break;
	case 5:/*Threshold values*/
		sprintf(filename,"%s_Reconstructed_ThresholdImage.ppm", output_fn);
		break;
	}

for(i=0;i<height_subimages;i++){/*Height coordinate of subimage*/
	unsigned int HeightBlockStart = i * SHIFT;
	for (j=0;j<width_subimages;j++){/*Width coordinate of subimage*/
		unsigned int WidthBlockStart = j * SHIFT;
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			unsigned int x = (k + HeightBlockStart);
			for(l=0;l<WidthBlockSize;l++){
				unsigned int y = (l + WidthBlockStart);
				register unsigned int xyM = (y + x * width) * 3;

				switch(type){
					case 0:/*Red values*/
						data_buffer[xyM + 0] = subimage_data[i][j][k][l][0];
						data_buffer[xyM + 1] = 0;
						data_buffer[xyM + 2] = 0;
						break;
					case 1:/*Green values*/
						data_buffer[xyM + 0] = 0;
						data_buffer[xyM + 1] = subimage_data[i][j][k][l][1];
						data_buffer[xyM + 2] = 0;
						break;
					case 2:/*Blue values*/
						data_buffer[xyM + 0] = 0;
						data_buffer[xyM + 1] = 0;
						data_buffer[xyM + 2] = subimage_data[i][j][k][l][2];
						break;
					case 3:/*Grey values*/
						data_buffer[xyM + 0] = subimage_data[i][j][k][l][3];
						data_buffer[xyM + 1] = subimage_data[i][j][k][l][3];
						data_buffer[xyM + 2] = subimage_data[i][j][k][l][3];
						break;
					case 4:/*RGB values*/
						data_buffer[xyM + 0] = subimage_data[i][j][k][l][0];
						data_buffer[xyM + 1] = subimage_data[i][j][k][l][1];
						data_buffer[xyM + 2] = subimage_data[i][j][k][l][2];
						break;
					case 5:/*Threshold values*/
						data_buffer[xyM + 0] = subimage_data[i][j][k][l][4];
						data_buffer[xyM + 1] = subimage_data[i][j][k][l][4];
						data_buffer[xyM + 2] = subimage_data[i][j][k][l][4];
						break;
					}
				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/

save_ppm(filename, width, height, data_buffer);

free(data_buffer);

return TRUE;
}


int final_stage(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int HeightBlockSize = 0;
unsigned int WidthBlockSize = 0;

unsigned int data_mem_alloc = 0;

char filename[128];
memset(filename, '\0', sizeof(filename));

unsigned char *data_buffer=NULL;
data_mem_alloc = (width * height);
data_buffer = (unsigned char *)malloc(data_mem_alloc * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("final_stage:Could not allocate %d bytes.\n", (data_mem_alloc * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	for(i=0;i<data_mem_alloc;i++)
		data_buffer[i] = 0;
	}

unsigned char **data_buffer_2D=NULL;
data_buffer_2D = (unsigned char **)malloc(height * sizeof(unsigned char *));
if (data_buffer_2D == NULL){
	printf("final_stage: Could not allocate %d bytes.\n", (height * sizeof(unsigned char *)));
	return FALSE;
	}
else{
	for (i=0;i<height;i++){
		data_buffer_2D[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
		if (data_buffer_2D[i] == NULL){
			printf("final_stage: Could not allocate %d bytes for i=%d index.\n", (width * sizeof(unsigned char)), i);
			return FALSE;
			}
		else{
			for(j=0;j<width;j++){
				data_buffer_2D[i][j] = 0;
				}
			}
		}
	}

sprintf(filename,"%s_alg1_Final.pgm", output_fn);

for(i=0;i<height_subimages;i++){/*Height coordinate of subimage*/
	unsigned int HeightBlockStart = i * SHIFT;
	for (j=0;j<width_subimages;j++){/*Width coordinate of subimage*/
		unsigned int WidthBlockStart = j * SHIFT;
		/*------------------------------------------------*/
		Calculate_Block_Size(i, j, height, width, &HeightBlockSize, &WidthBlockSize);
		/*------------------------------------------------*/

		for(k=0;k<HeightBlockSize;k++){
			unsigned int x = (k + HeightBlockStart);
			for(l=0;l<WidthBlockSize;l++){
				unsigned int y = (l + WidthBlockStart);
				register unsigned int xyM = (y + x * width);

				if (subimage_data[i][j][k][l][3] >= subimage_data[i][j][k][l][4]){
					data_buffer[xyM] = 255;
					}
				else{
					data_buffer[xyM] = 0;
					}
				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/


transform_1D_to_2D_Binary_Images(data_buffer, width, height, data_buffer_2D);
erode(data_buffer_2D, width, height, 1);
dilate(data_buffer_2D, width, height, 1);
for (i=0;i<height;i++){
	for(j=0;j<width;j++){
		register unsigned int idx = (j + i*width);
		data_buffer[idx] = data_buffer_2D[i][j];
		}
	}




save_pgm(filename, width, height, data_buffer);

for (i=0;i<height;i++)
	free(data_buffer_2D[i]);
free(data_buffer_2D);
free(data_buffer);


return TRUE;

}


