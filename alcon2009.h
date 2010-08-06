/* ############################################################################
Name           : alcon2009.h
Company        : ALCON 2009
Project        : Finding Clones
Programmer     : ALCON 2009
Revisor        : S. Vagionitis
Description    : Description of basic structures and basic functions declaration.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  02/06/2010     Creation

############################################################################ */

#ifndef __ALCON2009_H__
#define __ALCON2009_H__

/* line width for boundingbox */
#define LINE_WIDTH 4

#define ALLOW_PRINTF	TRUE

char output_fn[128];

typedef struct object_t {
	int label;	/* label */
	int rep;	/* representative flag(1:yes,0:no) */
	int x1, y1;	/* upper-left corner */
	int x2, y2;	/* lower-right corner */
} object;

unsigned char *load_ppm(const char *filename, int *width, int *height);
unsigned char *load_pgm(const char *filename, int *width, int *height);
int save_ppm(const char *filename, int width, int height, unsigned char *data);
int save_pgm(const char *filename, int width, int height, unsigned char *data);

object *my_alg_level1(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);
/*object *my_alg_level2(unsigned char *image, int width, int height, int *n_object);*/
object *my_alg_level2(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);
/*object *my_alg_level3(unsigned char *image, int width, int height, int *n_object);*/
object *my_alg_level3(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);

/*
object *my_alg_level1(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);
object *my_alg_level2(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);
object *my_alg_level3(unsigned char *image, unsigned char *mask, int width, int height, int *n_object);
*/
object *read_matlab_result(const char *, int *);
void evaluate(object *c, int n_c, const char *filename);

#endif

