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


/* ############################################################################
Name           : create_sub_images
Description    : 


Arguments          Type                Description
================== =================== ========================================
image_data(IN)     unsigned char *     Image data.
width(IN)          int                 Width of image.
height(IN)         int                 Height of image.
shift_step(IN)     unsigned int        Number of pixels to shift the 
                                       M x M window.
subimage_data(OUT) unsigned char ***** Subimages data.

Return Values                          Description
====================================== ========================================

Globals            Type            Description
================== =============== ============================================

Locals             Type            Description
================== =============== ============================================
i, j, k, l, m      unsigned int
step_per_subimage  unsigned int
width_subimages    unsigned int
height_subimages   unsigned int

############################################################################ */
int create_sub_images(unsigned char *image_data, int width, int height, unsigned int shift_step, unsigned char *****subimage_data)
{
unsigned int i = 0, j = 0, k = 0, l = 0, m = 0;

unsigned int step_per_subimage = M / shift_step;
unsigned int width_subimages = step_per_subimage * (width / M);
unsigned int height_subimages = step_per_subimage * (height / M);


printf("shift_step = %d, width = %d,  height = %d\n", shift_step, width, height);
printf("step_per_subimage = %d,  width_subimages = %d,  height_subimages = %d\n", step_per_subimage, width_subimages, height_subimages);


/* Allocate memory for subimage data*/
subimage_data = (unsigned char *****)malloc(width_subimages * sizeof(unsigned char ****));
if (subimage_data == NULL){
	printf("Could not allocate %d bytes.\n", (width_subimages * sizeof(unsigned char ****)));
	return FALSE;
	}
else{
	for (i = 0;i<width_subimages;i++){
		subimage_data[i] = (unsigned char ****)malloc(height_subimages * sizeof(unsigned char ***));
		if (subimage_data[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", (height_subimages * sizeof(unsigned char ***)), i);
			return FALSE;
			}
		else{
			for (j=0;j<height_subimages;j++){
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
								subimage_data[i][j][k][l] = (unsigned char *)malloc(3 * sizeof(unsigned char));
								if (subimage_data[i][j][k][l] == NULL){
									printf("Could not allocate %d bytes for l=%d index.\n", (3 * sizeof(unsigned char)), l);
									return FALSE;
									}
								else{
									/*Initialize subimage data.*/
									for(m=0;m<3;m++){
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

	printf("Allocated %d bytes.\n", (width_subimages * height_subimages * M * M * sizeof(unsigned char)));
	}/*else subimage_data*/

unsigned int x = 0, y = 0;
unsigned int ssi = 0, ssj = 0;
/* Populate subimage data*/
for(i=0;i<width_subimages;i++){/*Width coordinate of subimage*/
	ssi = shift_step*i;
	for (j=0;j<height_subimages;j++){/*Height coordinate of subimage*/
		ssj = shift_step*j;
		for(k=0;k<M;k++){
			for(l=0;l<M;l++){
				for(m=0;m<3;m++){
					x = (k + ssi);
					y = (l + ssj);
					subimage_data[i][j][k][l][m] = image_data[(y + x*M)*3 + m];
					printf("[%d %d %d %d %d] - [%d %d] %d\n", i, j, k, l, m, x, y, ((x*M + y)*3+m));
					}/*m*/
				}/*l*/
			}/*k*/
		}/*j*/
	}/*i*/


return TRUE;
}
