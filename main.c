/* ############################################################################
Name           : main.c
Company        : ALCON 2009
Project        : Finding Clones
Programmer     : ALCON 2009
Revisor        : S. Vagionitis
Description    : Main function

Procedure                     Description
============================= =================================================


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alcon2009.h"
#include "pnm.h"

/* line color for each cluster */
static unsigned char ALCON2009_COL_R[10] = {255,   0,   0, 255,   0, 255, 255,   0, 127, 0};
static unsigned char ALCON2009_COL_G[10] = {  0, 255,   0, 255, 255,   0, 127, 255,   0, 0};
static unsigned char ALCON2009_COL_B[10] = {  0,   0, 255,   0, 255, 255,   0, 127, 255, 0};


/*-----------------------------------------------------------
 * time measuring function 
 * For ANSI C only, define ALCON2009
 *-----------------------------------------------------------*/
/* #define ALCON2009 */
#ifdef ALCON2009
	#include <time.h>
	static double alcon2009_measure_time()
	{
		static double past = 0.0;
		double curr = clock();
		double time = (curr - past) / CLOCKS_PER_SEC;
		past = curr;
		return time;
	}
#elif WIN32 /* if windows.h is available*/
	#include <windows.h>
	static double alcon2009_measure_time()
	{
		static LARGE_INTEGER past;
		LARGE_INTEGER curr;
		LARGE_INTEGER freq;
		double time;

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&curr);

		time = (double)(curr.QuadPart-past.QuadPart) / freq.QuadPart;
		past = curr;

		return time;
	}
#else /* for UNIX */
	#include <sys/time.h>
	#include <sys/resource.h>
	static double alcon2009_measure_time()
	{
		static struct timeval past;
		struct timeval curr;
		struct rusage  temp;
		double time;

		getrusage(RUSAGE_SELF, &temp);
		curr = temp.ru_utime;

		time = (curr.tv_sec+(double)curr.tv_usec*1e-6) - (past.tv_sec+(double)past.tv_usec*1e-6);
		past = curr;
		
		return time;
	}
#endif

int create_result_rect(char *, int, int, object *, int);
int create_correct_rect(const char *, char *, int, int);

/* ############################################################################
Name           : alcon2009_draw_rectangle
Description    : Draw a rectangle by specifying its color(r,g,b), upper-left 
                 corner(x1, y1), and lower-right corner(x2, y2).

Arguments      Type             Description
============== ================ ===============================================
image          unsigned char *  The image data.
width          int              The width of the image.
height         int              The height of the image.
x1             int              The upper-left corner x-coordinate.
y1             int              The upper-left corner y-coordinate.
x2             int              The lower-left corner x-coordinate.
y2             int              The lower-left corner y-coordinate.
r, g, b        unsigned char    The red, green and blue values of the 
                                rectangle.

Return Values                   Description
=============================== ===============================================

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================
i              int              Counter for y coordinates.
w              int              Counter for the line width.
xx1            int              The values of x-coordinate of upper-left 
                                corner.
xx2            int              The values of x-coordinate of lower-right 
                                corner.
yy1            int              The values of y-coordinate of upper-left 
                                corner.
yy2            int              The values of y-coordinate of lower-right 
                                corner.

############################################################################ */
static void alcon2009_draw_rectangle
	(unsigned char *image, int width, int height, 
	 int x1, int y1, int x2, int y2, 
	 unsigned char r, unsigned char g, unsigned char b)
{
	int i, w;

	for (i = y1; i <= y2; i++) {
		for (w = 0; w < LINE_WIDTH; w++) {
			int xx1 = x1-w;
			int xx2 = x2+w;
			if (y1 >= 0 && y1 < height) {
				if (xx1 >= 0 && xx1 < width) {
					image[(xx1+i*width)*3+0] = r;
					image[(xx1+i*width)*3+1] = g;
					image[(xx1+i*width)*3+2] = b;
				}
				if (xx2 >= 0 && xx2 < width) {
					image[(xx2+i*width)*3+0] = r;
					image[(xx2+i*width)*3+1] = g;
					image[(xx2+i*width)*3+2] = b;
				}
			}
		}
	}

	for (i = x1-LINE_WIDTH+1; i <= x2+LINE_WIDTH-1; i++) {
		for (w = 0; w < LINE_WIDTH; w++) {
			int yy1 = y1-w;
			int yy2 = y2+w;
			if (x1 >= 0 && x1 < width) {
				if (yy1 >= 0 && yy1 < height) {
					image[(i+yy1*width)*3+0] = r;
					image[(i+yy1*width)*3+1] = g;
					image[(i+yy1*width)*3+2] = b;
				}
				if (yy2 >= 0 && yy2 < height) {
					image[(i+yy2*width)*3+0] = r;
					image[(i+yy2*width)*3+1] = g;
					image[(i+yy2*width)*3+2] = b;
				}
			}
		}
	}
}



/********************************************************************
 * main function
 * Usage:
 *   ./alcon2009 [level(1|2|3)] [input image] {[mask image(for level 1)]}
 ********************************************************************/
/* ############################################################################
Name           : main
Description    : The main function.

Arguments      Type             Description
============== ================ ===============================================

Return Values                   Description
=============================== ===============================================

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================

############################################################################ */
int main(int argc, char *argv[])
{
	int i;
	int w_img, h_img, w_mask, h_mask, n_object;
	unsigned char *data, *mask;
	object *obj;
	double time;
	char *input_filename;
	/*char result_filename[128];*/

	int level = 0;
	int eval  = 0;
	char *ground_truth_filename = NULL;
	
	
	/* program option analysis */
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'e') {
				eval = 1;
				ground_truth_filename = argv[++i];
			} else if (argv[i][1] == 'l') {
				level = argv[++i][0];
			}
		} else {
			break;
		}
	}

	if (level == 0) {
		fprintf(stderr, "Level has not been specified.\n");
		return -1;
	}

	if (level == '1' || level == '2' || level == '3') {
		printf("Input image file : %s\n", argv[i+0]);
		printf("Mask image file  : %s\n", argv[i+1]);
		data = load_ppm(argv[i+0], &w_img, &h_img);
		mask = load_pgm(argv[i+1], &w_mask, &h_mask);

		if (w_img != w_mask || h_img != h_mask) {
			fprintf(stderr, "Size mismatch between the target image and mask image\n");
			return -1;
		}
		input_filename = argv[i];
	}/* else if (level == '2' || level == '3') {
		printf("Input image file : %s\n", argv[i]);
		data = load_ppm(argv[i], &w_img, &h_img);
		mask = NULL;
		input_filename = argv[i];
	} */else {
		fprintf(stderr, "Invalid level\n");
		return -2;
	}

	if (!data) {
		fprintf(stderr, "cannot load the target image\n");
		return -3;
	}

	if (level == '1') {
		alcon2009_measure_time();
		obj = my_alg_level1(data, mask, w_img, h_img, &n_object);
		time = alcon2009_measure_time();
	} else if (level == '2') {
		alcon2009_measure_time();
		/*obj = my_alg_level2(data, w_img, h_img, &n_object);*/
		obj = my_alg_level2(data, mask, w_img, h_img, &n_object);
		for (i = 0; i < n_object; i++) {
			printf("%d %d %d %d %d %d\n", i, obj[i].label, obj[i].x1, obj[i].y1, obj[i].x2, obj[i].y2);
			}
		if (obj == NULL)
			return -1;
		time = alcon2009_measure_time();
	} else {
		alcon2009_measure_time();
		/*obj = my_alg_level3(data, w_img, h_img, &n_object);*/
		obj = my_alg_level3(data, mask, w_img, h_img, &n_object);
		for (i = 0; i < n_object; i++) {
			printf("%d %d %d %d %d %d\n", i, obj[i].label, obj[i].x1, obj[i].y1, obj[i].x2, obj[i].y2);
			}

		if (obj == NULL)
			return -1;
		time = alcon2009_measure_time();
	}

	create_result_rect(input_filename, w_img, h_img, obj, n_object);
	create_correct_rect(ground_truth_filename, input_filename, w_img, h_img);


	if (eval) {
		evaluate(obj, n_object, ground_truth_filename);
	} else { /* Show only the result and exit if evaluation is not needed */
		printf("  Result\n");
		printf("  Upper-left corner     Lower-right corner       Label\n");
		for (i = 0; i < n_object; i++) {
			int rep = (obj[i].rep) ? 'o' : ' ';
			printf("%c (%4d, %4d) (%4d, %4d)    %d\n",
				rep, obj[i].x1, obj[i].y1, obj[i].x2, obj[i].y2, obj[i].label);
		}
	}

	printf("Time: %lf [sec]\n", time);

	free(data);
	free(mask);
	free(obj);

	return 0;
}

int create_correct_rect(const char *filename, char *input_filename, int w_img, int h_img)
{
int i = 0, n = 0;
object *obj_gt;
char result_filename[128];

FILE *f = fopen(filename, "r");
fscanf(f, "%d\n", &n);

obj_gt = (object *)malloc(n * sizeof(object));

for (i = 0; i < n; i++) {
	fscanf(f, "%d %d %d %d %d\n", &obj_gt[i].x1, &obj_gt[i].y1, &obj_gt[i].x2, &obj_gt[i].y2, &obj_gt[i].label);
}

unsigned char *data = NULL;
data = (unsigned char *)malloc(3*w_img*h_img);
if (data == NULL){
	printf("create_correct_rect:Cannot allocate %d bytes for memory.\n", (3*w_img*h_img));
	exit(-1);
	}

data = load_ppm(input_filename, &w_img, &h_img);
for (i = 0; i < n; i++) {
	int c = obj_gt[i].label % 10; /* return to the original range, if it exceeds 10 */
	unsigned char r = ALCON2009_COL_R[c];
	unsigned char g = ALCON2009_COL_G[c];
	unsigned char b = ALCON2009_COL_B[c];

	alcon2009_draw_rectangle(data, w_img, h_img, obj_gt[i].x1, obj_gt[i].y1, obj_gt[i].x2, obj_gt[i].y2, r, g, b);
}
memcpy(result_filename, input_filename, sizeof(result_filename));
result_filename[strlen(result_filename)-4] = '\0';
sprintf(result_filename, "%s_correct.ppm", result_filename);
save_ppm(result_filename, w_img, h_img, data);

free(obj_gt);
free(data);
return 1;
}


int create_result_rect(char *input_filename, int w_img, int h_img, object *obj, int n)
{
int i = 0;
char result_filename[128];

unsigned char *data = NULL;
data = (unsigned char *)malloc(3*w_img*h_img);
if (data == NULL){
	printf("create_result_rect:Cannot allocate %d bytes for memory.\n", (3*w_img*h_img));
	exit(-1);
	}

data = load_ppm(input_filename, &w_img, &h_img);
for (i = 0; i < n; i++) {
	int c = obj[i].label % 10; /* return to the original range, if it exceeds 10 */
	unsigned char r = ALCON2009_COL_R[c];
	unsigned char g = ALCON2009_COL_G[c];
	unsigned char b = ALCON2009_COL_B[c];

	alcon2009_draw_rectangle(data, w_img, h_img, obj[i].x1, obj[i].y1, obj[i].x2, obj[i].y2, r, g, b);
}
memcpy(result_filename, input_filename, sizeof(result_filename));
result_filename[strlen(result_filename)-4] = '\0';
sprintf(result_filename, "%s_result.ppm", result_filename);
save_ppm(result_filename, w_img, h_img, data);

free(data);
return 1;
}
