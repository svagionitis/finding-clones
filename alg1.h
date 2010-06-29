/* ############################################################################
Name           : alg1.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for alg_lvl1.c.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */
#ifndef __ALG1_H__
#define __ALG1_H__

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define PI  		3.1415926535897932384626433832795
#define RAD(x)		((x)*PI/180.0)
#define MIN(a,b)	((a)<=(b)?(a):(b))
#define MAX(a,b)	((a)>=(b)?(a):(b))
#define MIN3(a,b,c)	((a)<=(b) ? (a)<=(c)?(a):(c) : (b)<=(c)?(b):(c) )
#define MAX3(a,b,c)	((a)>=(b) ? (a)>=(c)?(a):(c) : (b)>=(c)?(b):(c) )


#define GREYSCALE1(r, g, b)	((unsigned char)(0.2126 * (double)(r)) + \
                                 (unsigned char)(0.7152 * (double)(g)) + \
                                 (unsigned char)(0.0722 * (double)(b)))

#define GREYSCALE2(r, g, b)	((unsigned char)(0.2989 * (double)(r)) + \
                                 (unsigned char)(0.5870 * (double)(g)) + \
                                 (unsigned char)(0.1140 * (double)(b)))


/*
 * M x M regions that image will be divided. 
 * We could use also 24, 18 and 12 as stated in the paper.
 */
#define M 32

/*
 * Number of pixels to shift the M x M window.
 */
#define SHIFT (M/2)

/*
 * 8-bit colors
 */
#define COLORS 256


/*
 * For Step 5: Repeat steps 2 through 4 until the difference 
 * in T in successive iterations is smaller than a predefined 
 * parameter DIFF_T.
 */
#define DIFF_T 0.01

/*Structure for histogram*/
typedef struct histogram_t{
	unsigned int num_pixels;
	float freq;
	}histogram;

int create_sub_images(unsigned char *, int, int, unsigned int *, unsigned int *);
int free_mem_subimages(int, int, unsigned int, unsigned int);
int export_ppm_subimages(unsigned char, int, int, unsigned int, unsigned int);

int calculate_histogram(unsigned char, int, int, unsigned int, unsigned int);
int free_mem_histogram(unsigned int, unsigned int);

int calculate_threshold(int, int, unsigned int, unsigned int);
int calculate_min_max_hist_threshold(unsigned int, unsigned int, unsigned char *, unsigned char *);
int basic_global_thresholding_algorithm(unsigned int, unsigned int, float, unsigned char *);

int calculate_threshold_with_interpolation(unsigned char, int, int, unsigned int, unsigned int);
int bilinear_interpolation_with_weights_propotional_to_square_of_distance(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char*);
int bilinear_interpolation_with_weights_propotional_to_distance(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char*);
int linear_interpolation_in_2d_data(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned char*);
int reconstruct_image_from_subimages(unsigned char, int, int, unsigned int, unsigned int);
int final_stage(int, int, unsigned int, unsigned int);

extern unsigned char *****subimage_data;
extern histogram ***hist_data;
extern unsigned char **Ts;

#endif

