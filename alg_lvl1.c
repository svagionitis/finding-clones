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
#include <time.h>
#include <limits.h>/*UINT_MAX*/

#include "alg_lvl1.h"
#include "alcon2009.h"/*For save_ppm function*/

unsigned char *****subimage_data;
histogram ***hist_data;

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

	printf("Allocated %d bytes for RGB+Greyscale subimages data.\n", (((wdS * M) + wmS) * ((hdS * M) + hmS) * 4 * sizeof(unsigned char)));
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
Name           : free_sub_images_mem
Description    : Free memory from subimages.

Arguments             Type                Description
===================== =================== =====================================
width(IN)             int                 Width of image.
height(IN)            int                 Height of image.
width_subimages(IN)   unsigned int*       Width coordinate of subimage.
height_subimages(IN)  unsigned int*       Height coordinate of subimage.

Return Values                             Description
========================================= =====================================
TRUE                                      If all goes well.


Globals               Type                Description
===================== =================== =====================================

Locals                Type                Description
===================== =================== =====================================
i, j, k, l, m         unsigned int        General purpose indexes.

############################################################################ */
int free_sub_images_mem(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
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
Description    : 

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


printf("[%d %d][%u %u][%u %u][%u %u]\n", width, height, width_subimages, height_subimages, wdS, wmS, hdS, hmS);

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

Locals                Type                Description
===================== =================== =====================================
i, j, k, l            unsigned int        General purpose indexes.

############################################################################ */
int calculate_threshold(int width, int height, unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;

unsigned char **Ts;

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

		/*********************Begin Step 1*********************/
		/*Find min and max value of histogram*/
		unsigned char max_gry_level = 0, min_gry_level = 0;
		unsigned int max_hist = 0, min_hist = UINT_MAX;
		for (k=0;k<COLORS;k++){
			if (hist_data[i][j][k].num_pixels > max_hist){
				max_hist = hist_data[i][j][k].num_pixels;
				max_gry_level = k;
				}
			else if (hist_data[i][j][k].num_pixels <= min_hist){
				min_hist = hist_data[i][j][k].num_pixels;
				min_gry_level = k;
				}
			}/*for k*/
		/*Find min and max value of histogram*/


		/*Initial value of T is the mean value of min and max of grey levels*/
		float Tstart = ((float)(max_gry_level + min_gry_level) / 2);
		/*Initial value of T is a random value between 0 and COLORS-1*/
		/*unsigned char Tstart = (unsigned char)((float)(COLORS*rand())/(RAND_MAX+1.0));*/
		/*********************End Step 1*********************/
		float Tend = 0.0;
		while(fabs(Tstart - Tend) > DIFF_T){/*Step 5*/
			memcpy(&Tstart, &Tend, sizeof(Tend));
			/*********************Begin Step 2 & 3*********************/
			/*
			* Calculate the average grey level values mi1 and mi2 for the pixels 
			* in regions G1 and G2.
			*/
			unsigned int mi1_values = 0, mi2_values = 0;
			unsigned int count_mi1 = 0, count_mi2 = 0;
			unsigned char mi1 = 0, mi2 = 0;
			for (k=0;k<COLORS;k++){
				if (k > (unsigned int)Tstart){/*G1 region*/
					mi1_values += hist_data[i][j][k].num_pixels;
					count_mi1++;
					}
				else if (k <= (unsigned int)Tstart){/*G2 region*/
					mi2_values += hist_data[i][j][k].num_pixels;
					count_mi2++;
					}
				}/*for k*/
			mi1 = (unsigned char)((float)mi1_values / count_mi1);
			mi2 = (unsigned char)((float)mi2_values / count_mi2);
			/*********************End Step 2 & 3*********************/
	
			/*********************Begin Step 4*********************/
			Tend = ((float)(mi1 + mi2) / 2);
			/*********************End Step 4*********************/
			}/*while Step 5*/

		}/*for j*/
	}/*for i*/

return TRUE;
}

