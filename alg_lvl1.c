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

#include "alg_lvl1.h"
#include "alcon2009.h"

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

unsigned int step_per_subimage = M / SHIFT_STEP;
(*width_subimages) = step_per_subimage * (width / M);
(*height_subimages) = step_per_subimage * (height / M);


printf("shift_step = %d, width = %d,  height = %d\n", SHIFT_STEP, width, height);
printf("step_per_subimage = %d,  width_subimages = %d,  height_subimages = %d\n", step_per_subimage, (*width_subimages), (*height_subimages));


/* Allocate memory for subimage data*/
subimage_data = (unsigned char *****)malloc((*width_subimages) * sizeof(unsigned char ****));
if (subimage_data == NULL){
	printf("Could not allocate %d bytes.\n", ((*width_subimages) * sizeof(unsigned char ****)));
	return FALSE;
	}
else{
	for (i = 0;i<(*width_subimages);i++){
		subimage_data[i] = (unsigned char ****)malloc((*height_subimages) * sizeof(unsigned char ***));
		if (subimage_data[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", ((*height_subimages) * sizeof(unsigned char ***)), i);
			return FALSE;
			}
		else{
			for (j=0;j<(*height_subimages);j++){
				subimage_data[i][j] = (unsigned char ***)malloc(M * sizeof(unsigned char **));
				if (subimage_data[i][j] == NULL){
					printf("Could not allocate %d bytes for j=%d index.\n", (M * sizeof(unsigned char **)), j);
					return FALSE;
					}
				else{
					for(k=0;k<M;k++){
						subimage_data[i][j][k] = (unsigned char **)malloc(M * sizeof(unsigned char *));
						if (subimage_data[i][j][k] == NULL){
							printf("Could not allocate %d bytes for k=%d index.\n", (M * sizeof(unsigned char)), k);
							return FALSE;
							}
						else{
							for(l=0;l<M;l++){
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

	printf("Allocated %d bytes for RGB+Greyscale subimages data.\n", ((*width_subimages) * (*height_subimages) * M * M * 4 * sizeof(unsigned char)));
	}/*else subimage_data*/

unsigned int x = 0, y = 0;
unsigned int ssi = 0, ssj = 0;
/* Populate subimage data*/
for(i=0;i<(*width_subimages);i++){/*Width coordinate of subimage*/
	ssi = SHIFT_STEP*i;
	for (j=0;j<(*height_subimages);j++){/*Height coordinate of subimage*/
		//if (j == 2) return TRUE;
		ssj = SHIFT_STEP*j;
		for(k=0;k<M;k++){
			for(l=0;l<M;l++){
				x = (k + ssi);
				y = (l + ssj);
				subimage_data[i][j][k][l][0] = image_data[(y + x*M)*3 + 0];
				subimage_data[i][j][k][l][1] = image_data[(y + x*M)*3 + 1];
				subimage_data[i][j][k][l][2] = image_data[(y + x*M)*3 + 2];

				r = subimage_data[i][j][k][l][0];
				g = subimage_data[i][j][k][l][1];
				b = subimage_data[i][j][k][l][2];
				subimage_data[i][j][k][l][3] = GREYSCALE1(r, g, b);
				//printf("[%d %d %d %d %d] - [%d %d] %d\n", i, j, k, l, m, x, y, (y + x*M)*4 + m);

				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/


return TRUE;
}


/* ############################################################################
Name           : create_sub_images
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
int export_ppm_subimages_rgb(unsigned int width_subimages, unsigned int height_subimages)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;
char filename[128];

unsigned char *data_buffer;
data_buffer = (unsigned char *)malloc((3 * M * M) * sizeof(unsigned char));
if (data_buffer == NULL){
	printf("Could not allocate %d bytes.\n", ((3 * M * M) * sizeof(unsigned char)));
	return FALSE;
	}
else{/*Initialize data buffer*/
	printf("Mem alloc %d bytes.\n", (3 * M * M) * sizeof(unsigned char));
	for(i=0;i<(3 * M * M);i++)
		data_buffer[i] = 0;
	}

unsigned int x = 0;
for(i=0;i<width_subimages;i++){/*Width coordinate of subimage*/
	for (j=0;j<height_subimages;j++){/*Height coordinate of subimage*/
		if (j == 10) return TRUE;
		for(k=0;k<M;k++){
			for(l=0;l<M;l++){
				data_buffer[(l + k*M)*3 + 0] = subimage_data[i][j][k][l][0];
				data_buffer[(l + k*M)*3 + 1] = subimage_data[i][j][k][l][1];
				data_buffer[(l + k*M)*3 + 2] = subimage_data[i][j][k][l][2];
				//printf("[%d %d, %d %d, %d] %d\n", i, j, k, l, m, ((l + k*M)*3 + m));
				}/*l*/
			}/*k*/
		sprintf(filename,"%03u_%03u_Subimage[%dx%d].ppm",i, j, M, M);
		save_ppm(filename, M, M, data_buffer);
		for(x=0;x<(3 * M * M);x++)
			data_buffer[x] = 0;
		}/*j*/
	}/*i*/



return TRUE;
}

/*
int calculate_histogram(unsigned char type)
{
}
*/

