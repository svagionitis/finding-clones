/* ############################################################################
Name           : pnm.c
Company        : ALCON 2009
Project        : Finding Clones
Programmer     : ALCON 2009
Revisor        : S. Vagionitis
Description    : PNM Functions

Procedure                     Description
============================= =================================================
read_pnm_header               Read a PNM file header.
load_ppm                      Read PPM binary file (needs to free the return 
                              pointer).
load_pgm                      Read PGM binary file (needs to free the return 
                              pointer).
save_ppm                      Save into PPM binary file.
save_pgm                      Save into PGM binary file.

Globals        Type           Description
============== ============== =================================================


Programmer     Date           Action
============== ============== =================================================
S. Vagionitis  04/06/2010     Creation

############################################################################ */

#define __PNM_C__
#include "pnm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* buffer size of file input */
#define BUF_SIZE 256


/* ############################################################################
Name           : read_pnm_header
Description    : Read a PNM file header. "P5" is header for PGM (grayscale), 
                 "P6" is header for PPM (color).


Arguments      Type           Description
============== ============== =================================================
f(IN)          FILE *         File descriptor of the image.
w(OUT)         int *          Width of the image.
h(OUT)         int *          Height of the image.
max(OUT)       int *          The maximum value of the colour components for 
                              the pixels.

Return Values                 Description
============================= =================================================
FALSE                         If wrong "magic" ppm identifier.
FALSE                         If width or height is less than zero.
FALSE                         If the maximum value of colour components for 
                              pixels is more than 255.
ret                           The "magic" ppm identifier, could be 6 or 5, from 
                              P6 or P5 respectively.

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================
buf            char[]         Buffer for image header.
ret            int            The "magic" ppm identifier, could be 6 or 5, from 
                              P6 or P5 respectively.
############################################################################ */
static int read_pnm_header(FILE *f, int *w, int *h, int *max)
{
	char buf[BUF_SIZE];
	
	int ret;

	fgets(buf, BUF_SIZE, f);
	if (strcmp(buf, "P6\n") == 0) {
		ret = 6;
	} else if (strcmp(buf, "P5\n") == 0) {
		ret = 5;
	} else {
		return FALSE;
	}

	do {
		fgets(buf, BUF_SIZE, f);
	} while (buf[0] == '#');

	sscanf(buf, "%d %d", w, h);
	if (*w < 0 || *h < 0) return FALSE;

	fgets(buf, BUF_SIZE, f);
	sscanf(buf, "%d", max);
	if (*max > 255) return FALSE;
	
	return ret;
}


/* ############################################################################
Name           : load_ppm
Description    : Read PPM binary file (needs to free the return pointer). Load 
                 the image data in memory.

Arguments      Type                 Description
============== ===================  ===========================================
filename(IN)   unsigned char *      Filename of the image file.
width(OUT)     int *                Width of the image.
height(OUT)    int *                Height of the image.

Return Values                       Description
==================================  ===========================================
data                                The image data.

Globals        Type                 Description
============== ===================  ===========================================

Locals         Type                 Description
============== ===================  ===========================================
data           unsigned char *      The image data.
max            int                  The maximum value of the colour components 
                                    for the pixels.
w              int                  The width of the image.
h              int                  The height of the image.
c              size_t               The total number of elements successfully 
                                    read from image file.

############################################################################ */
unsigned char *load_ppm(const char *filename, int *width, int *height)
{
	unsigned char *data = NULL;
	int max, w, h;
	size_t c;
	
	FILE *f = fopen(filename, "rb");
	if (!f) return NULL;

	if (read_pnm_header(f, &w, &h, &max) != 6) {
		fclose(f);
		return NULL;
	}

	data = (unsigned char *)malloc(3*w*h);
	c = fread(data, 1, 3*w*h, f);

	if (c != w*h*3) {
		free(data);
		data = NULL;
	}

	*width  = w;
	*height = h;
	
	return data;
}


/* ############################################################################
Name           : load_pgm
Description    : Read PGM binary file (needs to free the return pointer).Load 
                 the image data in memory.

Arguments      Type                 Description
============== ===================  ===========================================
filename(IN)   unsigned char *      Filename of the image file.
width(OUT)     int *                Width of the image.
height(OUT)    int *                Height of the image.

Return Values                       Description
==================================  ===========================================
data                                The image data.

Globals        Type                 Description
============== ===================  ===========================================

Locals         Type                 Description
============== ===================  ===========================================
data           unsigned char *      The image data.
max            int                  The maximum value of the colour components 
                                    for the pixels.
w              int                  The width of the image.
h              int                  The height of the image.
c              size_t               The total number of elements successfully 
                                    read from image file.

############################################################################ */
unsigned char *load_pgm(const char *filename, int *width, int *height)
{
	unsigned char *data = NULL;
	int max, w, h;
	size_t c;
	
	FILE *f = fopen(filename, "rb");
	if (!f) return NULL;

	if (read_pnm_header(f, &w, &h, &max) != 5) {
		fclose(f);
		return NULL;
	}

	data = (unsigned char *)malloc(w*h);
	c = fread(data, 1, w*h, f);

	if (c != w*h) {
		free(data);
		data = NULL;
	}

	*width  = w;
	*height = h;
	
	return data;
}


/* ############################################################################
Name           : save_ppm
Description    : Save into PPM binary file.

Arguments      Type                 Description
============== ===================  ===========================================
filename(IN)   unsigned char *      Filename of the image file.
width(IN)      int                  Width of the image.
height(IN)     int                  Height of the image.
data(IN)       unsigned char *      The image data.

Return Values                       Description
==================================  ===========================================
TRUE                                If everything OK.
FALSE                               If file descriptor is not OK or could not 
                                    write to the file.

Globals        Type                 Description
============== ===================  ===========================================

Locals         Type                 Description
============== ===================  ===========================================
f              FILE *               The image file descriptor.

############################################################################ */
int save_ppm(const char *filename, int width, int height, unsigned char *data)
{
	FILE *f = fopen(filename, "wb");
	if (!f) return FALSE;

	fprintf(f, "P6\n%d %d\n255\n", width, height);
	if (fwrite(data, 1, 3*width*height, f) != 3*width*height)
		return FALSE;

	return TRUE;
}


/* ############################################################################
Name           : save_pgm
Description    : Save into PGM binary file.

Arguments      Type                 Description
============== ===================  ===========================================
filename(IN)   unsigned char *      Filename of the image file.
width(IN)      int                  Width of the image.
height(IN)     int                  Height of the image.
data(IN)       unsigned char *      The image data.

Return Values                       Description
==================================  ===========================================
TRUE                                If everything OK.
FALSE                               If file descriptor is not OK or could not 
                                    write to the file.

Globals        Type                 Description
============== ===================  ===========================================

Locals         Type                 Description
============== ===================  ===========================================
f              FILE *               The image file descriptor.

############################################################################ */
int save_pgm(const char *filename, int width, int height, unsigned char *data)
{
	FILE *f = fopen(filename, "wb");
	if (!f) return FALSE;

	fprintf(f, "P5\n%d %d\n255\n", width, height);
	if (fwrite(data, 1, width*height, f) != width*height)
		return FALSE;

	return TRUE;
}
