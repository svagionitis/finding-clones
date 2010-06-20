/* ############################################################################
Name           : alg1.c
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

#define __ALG1_C__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <limits.h>/*UINT_MAX*/

#include "alg1.h"
#include "alcon2009.h"/*For save_ppm function*/

unsigned char *****subimage_data;
histogram ***hist_data;
unsigned char **Ts;

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
step_per_subimage     unsigned int        Shift step per subimage.

############################################################################ */
int create_sub_images(unsigned char *image_data, int width, int height, unsigned int *width_subimages, unsigned int *height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;

(*width_subimages) =  width / SHIFT;
(*height_subimages) = height / SHIFT;
unsigned int sub_hei = (*height_subimages);
unsigned int sub_wid = (*width_subimages);

printf("SHIFT= %d, Width= %d, Height= %d, Width_Sub= %d, Height_Sub= %d\n", SHIFT, width, height, sub_wid, sub_hei);

unsigned int wdS = (width / SHIFT) - 1;
unsigned int wmS = width % SHIFT;
unsigned int hdS = (height / SHIFT) - 1;
unsigned int hmS = height % SHIFT;

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
				if (i < hdS)
					h_mem_alloc = M;
				else
					h_mem_alloc = hmS;
				/*------------------------------------------------*/

				subimage_data[i][j] = (unsigned char ***)malloc(h_mem_alloc * sizeof(unsigned char **));
				if (subimage_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (h_mem_alloc * sizeof(unsigned char **)), j);
					return FALSE;
					}
				else{
					for(k=0;k<h_mem_alloc;k++){

						/*------------------------------------------------*/
						if (j < wdS)
							w_mem_alloc = M;
						else
							w_mem_alloc = wmS;
						/*------------------------------------------------*/

						subimage_data[i][j][k] = (unsigned char **)malloc(w_mem_alloc * sizeof(unsigned char *));
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

	printf("Allocated %d bytes for RGB+Greyscale subimages data.\n", (((wdS * M) + wmS) * ((hdS * M) + hmS) * 5 * sizeof(unsigned char)));
	}/*else subimage_data*/


/* Populate subimage data*/
for(i=0;i<sub_hei;i++){/*Height coordinate of subimage*/
	unsigned int ssi = SHIFT*i;
	for (j=0;j<sub_wid;j++){/*Width coordinate of subimage*/
		unsigned int ssj = SHIFT*j;
		
		/*------------------------------------------------*/
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){
			unsigned int x = (k + ssi);
			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				unsigned char r = 0, g = 0, b = 0;
				unsigned int y = (l + ssj);

				unsigned int xyM = (y + x * (width)) * 3;

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

############################################################################ */
int free_mem_subimages(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;

unsigned int wdS = (width / SHIFT) - 1;
unsigned int wmS = width % SHIFT;
unsigned int hdS = (height / SHIFT) - 1;
unsigned int hmS = height % SHIFT;

for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*------------------------------------------------*/
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
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
Name           : export_ppm_subimages_rgb
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


############################################################################ */
int export_ppm_subimages(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;
unsigned int data_mem_alloc = 0;
char filename[128];

unsigned int wdS = (width / SHIFT) - 1;
unsigned int wmS = width % SHIFT;
unsigned int hdS = (height / SHIFT) - 1;
unsigned int hmS = height % SHIFT;

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
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/

		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){
				unsigned int lkwma = (l + k*w_mem_alloc)*3;
				switch(type){
					case 0:/*Red values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][0];
						data_buffer[lkwma + 1] = 0;
						data_buffer[lkwma + 2] = 0;
						break;
					case 1:/*Green values*/
						data_buffer[lkwma + 0] = 0;
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][1];
						data_buffer[lkwma + 2] = 0;
						break;
					case 2:/*Blue values*/
						data_buffer[lkwma + 0] = 0;
						data_buffer[lkwma + 1] = 0;
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][2];
						break;
					case 3:/*Grey values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][3];
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][3];
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][3];
						break;
					case 4:/*RGB values*/
						data_buffer[lkwma + 0] = subimage_data[i][j][k][l][0];
						data_buffer[lkwma + 1] = subimage_data[i][j][k][l][1];
						data_buffer[lkwma + 2] = subimage_data[i][j][k][l][2];
						break;
					}
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
r, g, b               unsigned char       RGB values.
gry                   unsigned char       Grey value.

############################################################################ */
int calculate_histogram(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;

unsigned int pixel_value_counter[COLORS], max_pixels = 0;
memset(pixel_value_counter, 0, sizeof(pixel_value_counter));

unsigned int wdS = (width / SHIFT) - 1;
unsigned int wmS = width % SHIFT;
unsigned int hdS = (height / SHIFT) - 1;
unsigned int hmS = height % SHIFT;

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
	printf("Allocated %d bytes for Histogram data.\n", (height_subimages * width_subimages * COLORS * sizeof(histogram)));
	}/*else hist_data*/


for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*------------------------------------------------*/
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/
		
		memset(pixel_value_counter, 0, sizeof(pixel_value_counter));
		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/
			max_pixels = (h_mem_alloc*w_mem_alloc);
			for(l=0;l<w_mem_alloc;l++){
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
                 Processing, 2nd ed Prentice Hall, Upper Saddle River, NJ, 
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
			}/*else hist_data[i]*/
		}/*for i*/
	printf("Allocated %d bytes for Threshold data.\n", (height_subimages * width_subimages * sizeof(unsigned char)));
	}/*else hist_data*/


/* initialize random seed: */
srand (time(NULL));

for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*if (j == 4) return TRUE;*/

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
h_subim_index(IN)     unsigned char
w_subim_index(IN)     unsigned char
max_lvl_hist(OUT)     unsigned char *
min_lvl_hist(OUT)     unsigned char *

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.


Globals               Type                Description
===================== =================== =====================================
hist_data             histogram ***       The histogram of the subimages.

Locals                Type                Description
===================== =================== =====================================
i                     unsigned int        General purpose index.

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

/*printf("[%u %u] [max_pix_hist:%u min_pix_hist:%u]\n", h_subim_index, w_subim_index, max_pix_hist, min_pix_hist);*/

(*max_lvl_hist) = max_lvl;
(*min_lvl_hist) = min_lvl;

return TRUE;
}


/* ############################################################################
Name           : basic_global_thresholding_algorithm
Description    : 


Arguments             Type                Description
===================== =================== =====================================
h_subim_index(IN)     unsigned char
w_subim_index(IN)     unsigned char
Tstart(IN)            float
Ts(OUT)               unsigned char *

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.


Globals               Type                Description
===================== =================== =====================================
hist_data             histogram ***       The histogram of the subimages.

Locals                Type                Description
===================== =================== =====================================
i                     unsigned int        General purpose index.

############################################################################ */
int basic_global_thresholding_algorithm(unsigned int h_subim_index, unsigned int w_subim_index, float Tstart, unsigned char *Ts)
{
unsigned int i = 0;
float Tend = 0.0;
unsigned char first_run_flag = TRUE;
while(fabs(Tstart - Tend) > (float)DIFF_T){/*Step 5*/
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

############################################################################ */
int calculate_threshold_with_interpolation(unsigned char type, int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
unsigned int h_mem_alloc = 0;
unsigned int w_mem_alloc = 0;

unsigned int wdS = (width / SHIFT) - 1;
unsigned int wmS = width % SHIFT;
unsigned int hdS = (height / SHIFT) - 1;
unsigned int hmS = height % SHIFT;


/*
 * Assign in (0,0) position in a subimage the 
 * Threshold value that it was calculated before. 
 * The others values will be calculated using interpolation.
 */
for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){
		/*------------------------------------------------*/
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/
		for(k=0;k<h_mem_alloc;k++){
			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/
			for(l=0;l<w_mem_alloc;l++){
				subimage_data[i][j][0][0][4] = Ts[i][j];
				}/*for l*/
			}/*for k*/
		}/*for j*/
	}/*for i*/


for (i=0;i<height_subimages;i++){
	for (j=0;j<width_subimages;j++){

		/*------------------------------------------------*/
		if (i < hdS)
			h_mem_alloc = M;
		else
			h_mem_alloc = hmS;
		/*------------------------------------------------*/
		
		for(k=0;k<h_mem_alloc;k++){

			/*------------------------------------------------*/
			if (j < wdS)
				w_mem_alloc = M;
			else
				w_mem_alloc = wmS;
			/*------------------------------------------------*/

			for(l=0;l<w_mem_alloc;l++){

				switch(type){
					case 0:
						
						break;
					case 1:

						break;
					}

				}/*for l*/
			}/*for k*/
		}/*for j*/
	}/*for i*/



return TRUE;
}



int bilinear_interpolation_with_weights_propotional_to_square_of_distance(unsigned int h_subim_index, unsigned int w_subim_index, unsigned int x, unsigned int y)
{
unsigned char I1 = 0, I2 = 0, I3 = 0, I4 = 0;
float R1POW2 = 0.0, R2POW2 = 0.0, R3POW2 = 0.0, R4POW2 = 0.0;

I1 = subimage_data[h_subim_index][w_subim_index][0][0][4];
I2 = subimage_data[h_subim_index][w_subim_index + 1][0][0][4];
I3 = subimage_data[h_subim_index + 1][w_subim_index + 1][0][0][4];
I4 = subimage_data[h_subim_index + 1][w_subim_index][0][0][4];

R1POW2 = pow((x - 0), 2) + pow((y - 0), 2);
R2POW2 = pow((x - 0), 2) + pow((M - y), 2);
R3POW2 = pow((M - x), 2) + pow((M - y), 2);
R4POW2 = pow((M - x), 2) + pow((y - 0), 2);



return TRUE;
}


int bilinear_interpolation_with_weights_propotional_to_distance(unsigned int h_subim_index, unsigned int w_subim_index, unsigned int x, unsigned int y)
{

return TRUE;
}

