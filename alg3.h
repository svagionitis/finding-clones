/* ############################################################################
Name           : alg3.h
Company        : S. Vagionitis
Project        : Finding Clones
Programmer     : S. Vagionitis
Revisor        : S. Vagionitis
Description    : Header file for alg_lvl3.c.


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  10/06/2010     Creation

############################################################################ */
#ifndef __ALG3_H__
#define __ALG3_H__


#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define PI  		3.1415926535897932384626433832795
#define RAD(x)		((x)*PI/180.0)
#define DEG(x)		((x)*180.0/PI)
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

#pragma pack(1)

/*RGB color space*/
typedef struct rgb_t{
	unsigned char r, g, b;
	}RGB;

typedef struct cielab_flt{
	float L, a, b;/*Range L:0 - 100, a, b:-120 - 120*/
	}CIELab_float;

typedef struct cielab_uchr{
	unsigned char L8b, a8b, b8b;/*8-bit*/
	}CIELab_uchar;

/*CIE L*a*b* color space*/
typedef struct cielab_t{
	unsigned int C;
	float L, a, b;/*Range L:0 - 100, a, b:-120 - 120*/
	unsigned char L8b, a8b, b8b;/*8-bit*/
	}CIELab;

int transform_1D_to_2D_RGB(unsigned char *, int, int);
int allocate_mem_data_CIELAB(int, int);
int RGB_to_CIELAB(RGB, CIELab *);
int CIELAB_to_RGB(CIELab, RGB *);
int convert_RGB_to_CIELAB(int, int);
int first_derivative_CIELAB(unsigned char, int, int, unsigned int *);
int first_derivative_CIELAB_8bit(unsigned char, int, int, unsigned int *);
int calculate_histogram_of_gradient(int, int, unsigned int);
int histogram_analysis(int, int, unsigned int);
int calculate_area_under_histogram(unsigned int, unsigned int, unsigned int *);
int automatic_threshold_generation(unsigned int, unsigned int, unsigned int *);


extern RGB **data2D_RGB;
extern CIELab **data2D_CIELAB;
extern unsigned int **gradient_Map;
extern unsigned int *hist_gradient;

#endif

