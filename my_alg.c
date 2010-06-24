/* ############################################################################
Name           : my_alg.c
Company        : ALCON 2009
Project        : Finding Clones
Programmer     : ALCON 2009
Revisor        : S. Vagionitis
Description    : Algorithm Functions

Procedure                     Description
============================= =================================================
replace_obj_id                Replace objects with IDs.
assign_id                     Assign ID to each object(area).
find_rect                     Find a bounding box for each object.
calculate_area                Calculate the area of the object.
calculate_length              Calculate the perimeter length.
k_means                       Clustering by k-means algorithm.


Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  04/06/2010     Creation

############################################################################ */

#define __MY_ALG_C__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <float.h>

#include "alcon2009.h"
#include "alg1.h"
#include "alg2.h"
#include "alg3.h"

#define PI  3.1415926535897932384626433832795
#define RT2 1.4142135623730950488016887242097 /* = sqrt(2.0) */


/* number of clusters */
#define NUM_CLASS 2


/* prototypes for functions */
int  assign_id(unsigned char *img, int width, int height, int **obj_id);
void find_rect(int **obj_id, int width, int height, int n_object, object *obj);
void calculate_area(int **obj_id, int width, int height, int n_object, int *area);
void calculate_length(int **obj_id, int width, int height, int n_object, double *len, unsigned char *image);
void k_means(double *x, object *obj, int N);
object *baseline(unsigned char *, unsigned char *, int, int, int *);
int morphological_feature_circularity_index(int *, double *, int, double *);

/* non-implemented for level 2 and level 3 */
object *my_alg_level2(unsigned char *image, int width, int height, int *n_object) 
{

return NULL;
}

object *my_alg_level3(unsigned char *image, int width, int height, int *n_object) 
{

return NULL;
}


object *my_alg_level1(unsigned char *image, unsigned char *mask, int width, int height, int *n_object)
{

	/*********************************MYCODE*********************************/

/*
	unsigned int width_sub = 0, height_sub = 0;
	create_sub_images(image, width, height, &width_sub, &height_sub);
*/
	/*export_ppm_subimages(3, width, height, width_sub, height_sub);*/

/*
	calculate_histogram(3, width, height, width_sub, height_sub);

	calculate_threshold(width, height, width_sub, height_sub);

	calculate_threshold_with_interpolation(0, width, height, width_sub, height_sub);

	reconstruct_image_from_subimages(0, width, height, width_sub, height_sub);
	reconstruct_image_from_subimages(1, width, height, width_sub, height_sub);
	reconstruct_image_from_subimages(2, width, height, width_sub, height_sub);
	reconstruct_image_from_subimages(3, width, height, width_sub, height_sub);
	reconstruct_image_from_subimages(4, width, height, width_sub, height_sub);
	reconstruct_image_from_subimages(5, width, height, width_sub, height_sub);
*/
	/*free_mem_subimages(width, height, width_sub, height_sub);*/
	/*********************************MYCODE*********************************/


	return baseline(image, mask, width, height, n_object);
}



object *baseline(unsigned char *image, unsigned char *mask, int width, int height, int *n_object)
{
int i, j;
int n;	/* number of objects */

int    *area;	/* area */
double *len;	/* perimeter length */
double *circ;	/* circularity index */

object *obj;	/* for storing results */

int **obj_id = (int **)malloc(height*sizeof(int *));
if (obj_id == NULL){
	printf("Could not allocate %d bytes.\n", height*sizeof(int *));
	exit(0);
	}
else{
	for (i = 0; i < height; i++){
		obj_id[i] = (int *)malloc(width*sizeof(int));
		if (obj_id[i] == NULL){
			printf("Could not allocate %d bytes for i=%d index.\n", width*sizeof(int), i);
			exit(0);
			}
		}
	}

/* assign ID to each object and returns the number of objects */
n = assign_id(mask, width, height, obj_id);

/* allocate memories */
area = (int *)malloc(n * sizeof(int));
if (area == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n * sizeof(int)));
	exit(-1);
	}

len  = (double *)malloc(n * sizeof(double));
if (len == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n * sizeof(double)));
	exit(-1);
	}

circ = (double *)malloc(n * sizeof(double));
if (circ == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n * sizeof(double)));
	exit(-1);
	}

	
obj  = (object *)malloc(n * sizeof(object));
if (obj == NULL){
	printf("Cannot allocate %d bytes for memory.\n", (n * sizeof(object)));
	exit(-1);
	}

/* calcuate areas */
calculate_area(obj_id, width, height, n, area);

/* calcuate perimeter length */
calculate_length(obj_id, width, height, n, len, image);

/* calcuate cirularity index */
morphological_feature_circularity_index(area, len, n, circ);


/* k-means clustering */
k_means(circ, obj, n);

/* choose representatives (smallest number for each cluster) */
for (i = 0; i < n; i++) obj[i].rep = 0;
for (j = 0; j < NUM_CLASS; j++) {
	for (i = 0; i < n; i++) {
		if (obj[i].label == j) {
			obj[i].rep = 1;
			break;
		}
	}
}

/* find bounding box */
find_rect(obj_id, width, height, n, obj);

for (i = 0; i < height; i++) free(obj_id[i]);
free(obj_id);

free(area);
free(len);
free(circ);

*n_object = n;

return obj;

}

/* ############################################################################
Name           : replace_obj_id
Description    : Replace objects with IDs.

Arguments      Type             Description
============== ================ ===============================================
obj_id(IN,OUT) int **           The array of objects to be replaced.
n(IN)          int              ID to replace object.
t(IN)          int              Object to be replaced.
width(IN)      int              Width of object area.
height(IN)     int              Height of object area.

Return Values                   Description
=============================== ===============================================

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================
x, y           int              General purpose indexes.

############################################################################ */
void replace_obj_id(int **obj_id, int n, int t, int width, int height)
{
	int x, y;
	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			if (obj_id[y][x] == t) {
				obj_id[y][x] =  n;
			}
		}
	}
}


/*************************************************************
 * Assign ID to each object(area)                            *
 *************************************************************/
/* ############################################################################
Name           : assign_id
Description    : Assign ID to each object(area).

Arguments      Type             Description
============== ================ ===============================================
img(IN)        unsigned char *  Pixel values.
width(IN)      int              Width of object area.
height(IN)     int              Height of object area.
obj_id(IN)     int **           The array of objects to be replaced.

Return Values                   Description
=============================== ===============================================
count

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================
x, y           int              General purpose indexes.
s              int              Object to be replaced.
count
val            int              Count new objects.
u, l           int              Above pixel, left pixel.

############################################################################ */
int assign_id(unsigned char *img, int width, int height, int **obj_id)
{
	int x, y, s;
	int count, val = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {	
			obj_id[y][x] = 0;
		}
	}

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			/* pixel vaule is 0 in an object area */
			if (img[x+y*width] == 0 && obj_id[y][x] == 0) {
#if 0 /* 2009/5/8 revised */
				/* In an object area, search left and above */
				int u = obj_id[y-1][x  ];	/* above pixel */
				int l = obj_id[y  ][x-1];	/* left pixel*/
#endif
				int u = 0, l = 0;
				if (y > 0) u = obj_id[y-1][x  ];	/* above pixel */
				if (x > 0) l = obj_id[y  ][x-1];	/* leftpixel */

				if (u == 0 && l == 0)        { /* new object */
					obj_id[y][x] = ++val;
				} else if (u == l && u != 0) { /* the sambe object with the left and above pixels  */
					obj_id[y][x] = u;
				} else if (u != 0 && l == 0) { /* the same object with the above */
					obj_id[y][x] = u;
				} else if (u == 0 && l != 0) { /* the same object with the left pixel */
					obj_id[y][x] = l;
				} else if (u != 0 && l != 0) { /* the above and the left pixels are connected */
					if (u < l) { /* choose the smaller ID */
						obj_id[y][x] = u;
						replace_obj_id(obj_id, u, l, width, height);
					} else {
						obj_id[y][x] = l;
						replace_obj_id(obj_id, l, u, width, height);
					}
				}
			}
		}
	}

	/* re-assign IDs */
	count = 0;
	for (s = 1; s <= val; s++) {
		int n = 0;
		for (y = 0; y < height; y++){
			for (x = 0; x < width; x++){
				if (obj_id[y][x] == s) n += 1;
			}
		}

		if (n > 0) {
			count += 1;
			replace_obj_id(obj_id, count, s, width, height);
		}
	}

	return count;
}



/*************************************************************
 * Find a bounding box for each object                       *
 *************************************************************/
void find_rect(int **obj_id, int width, int height, int n_object, object *obj)
{
	int i, x, y;

	for (i = 0; i < n_object; i++) {
		obj[i].x1 = width;
		obj[i].y1 = height;
		obj[i].x2 = 0;
		obj[i].y2 = 0;
	}

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			int l = obj_id[y][x] - 1;
			if (l == -1) continue;

			if (obj[l].x1 > x) obj[l].x1 = x;
			if (obj[l].y1 > y) obj[l].y1 = y;
			if (obj[l].x2 < x) obj[l].x2 = x;
			if (obj[l].y2 < y) obj[l].y2 = y;
		}
	}
}


/*************************************************************
 * Calculate the area of the object                          *
 *************************************************************/
void calculate_area(int **obj_id, int width, int height, int n_object, int *area)
{
	int i, x, y;

	for (i = 0; i < n_object; i++) area[i] = 0;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			int l = obj_id[y][x];
			if (l == 0) continue;
			area[l-1] += 1;
		}
	}
}


/*************************************************************
 * Calculate the perimeter length                            *
 *************************************************************/
void calculate_length(int **obj_id, int width, int height, int n_object, double *len, unsigned char *image)
{
	static const int    dx[8] = { -1,  -1,  -1,   0,   1,   1,   1,   0};
	static const int    dy[8] = { -1,   0,   1,   1,   1,   0,  -1,  -1};
	static const double dl[8] = {RT2, 1.0, RT2, 1.0, RT2, 1.0, RT2, 1.0};
	static const int  next[8] = {  6,   0,   0,   2,   2,   4,   4,   6};

    int i, j;
	int  x = 0,  y = 0;
	int px = 0, py = 0;

	for(i = 0; i < n_object; i++) {
		int l = i + 1; /* ID of the object */
		int found = 0;
		int dir;

		len[i] = 0;
		
		/* search connected regions whose value are l */
		for (y = 0; y < height; y++) {
			for (x = 0; x < width; x++) {
				if (obj_id[y][x] == l) {
					px = x;
					py = y;
					found = 1;
					break;
				}
			}
			if (found) break;
		}

		/* trace countours by searching 8-neighbours */
		dir = 2;
		do {
			for (j = 0; j < 8; j++) {
				int a = (dir + j) % 8;
				if (obj_id[py+dy[a]][px+dx[a]] == l) {
					len[i] += dl[a];
					px += dx[a];
					py += dy[a];
					dir = next[a];
					
					break;
				}
			}
		} while (px != x || py != y);
	}
}



/*************************************************************
 * Clustering by k-means algorithm
 *************************************************************/
void k_means(double *x, object *obj, int n_object)
{
	static const double R = (double)NUM_CLASS / (RAND_MAX+1.0);

	int i, c;
	int    *num = (int    *)malloc(NUM_CLASS*sizeof(int));
	double *cen = (double *)malloc(NUM_CLASS*sizeof(double));

	srand((unsigned int)time(NULL));

	/*Assign labels/clusters randomly to the objects*/
	for (i = 0; i < n_object; i++) obj[i].label = (int)(rand()*R);

	while (1) {
		int count;

		for (c = 0; c < NUM_CLASS; c++) {
			num[c] =   0;
			cen[c] = 0.0;
		}

		/* find the cluster centroids */
		for (i = 0; i < n_object; i++) {
			num[obj[i].label] +=    1;
			cen[obj[i].label] += x[i];
		}

		for (c = 0; c < NUM_CLASS; c++) cen[c] /= num[c];

		/* re-assign clusters */
		count = 0;
		for (i = 0; i < n_object; i++) {
			double min = DBL_MAX;
			int min_class = 0;

			for (c = 0; c < NUM_CLASS; c++) {
				double distance = fabs(x[i] - cen[c]);
				if (distance < min) {
					min = distance;
					min_class = c;
				}
			}

			/* for stopping condition */
			if (obj[i].label == min_class) count += 1;

			obj[i].label = min_class;
		}

		/* check the stopping condition */
		if (count == n_object) break;
	}

	free(num);
	free(cen);
}



/* ############################################################################
Name           : morphological_feature_circularity_index
Description    : Calculate the circularity index which is a morphological 
                 feature. Values range from 1, for a perfect circle, to 0 
                 for a line. It is useful to give an impression of elongation 
                 as well as roughness of the object's shape. Other significant 
                 characteristics of this feature are that it is invariant to 
                 scale, translation and rotation.

Arguments      Type             Description
============== ================ ===============================================
area(IN)       int  *           Area covered by the objects.
perim(IN)      double *         Perimeter of the objects.
n_object(IN)   int              Number of objects from mask.
circ(OUT)      double *         Circularity index of objects.

Return Values                   Description
=============================== ===============================================
TRUE                            If everything is fine.

Globals        Type             Description
============== ================ ===============================================

Locals         Type             Description
============== ================ ===============================================
i              unsigned in      General purpose index.

############################################################################ */
int morphological_feature_circularity_index(int *area, double *perim, int n_object, double *circ)
{
unsigned int i = 0;

/* calcuate cirularity index */
for (i = 0; i < n_object; i++){
	circ[i] = 4.0*PI * area[i] / (perim[i]*perim[i]);
	}

return TRUE;
}

