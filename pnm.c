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
Description    : Read a PNM file header.

Arguments      Type           Description
============== ============== =================================================

Return Values                 Description
============================= =================================================

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================

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
Description    : Read PPM binary file (needs to free the return pointer).

Arguments      Type           Description
============== ============== =================================================

Return Values                 Description
============================= =================================================

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================

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
Description    : Read PGM binary file (needs to free the return pointer).

Arguments      Type           Description
============== ============== =================================================

Return Values                 Description
============================= =================================================

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================

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

Arguments      Type           Description
============== ============== =================================================

Return Values                 Description
============================= =================================================

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================

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

Arguments      Type           Description
============== ============== =================================================

Return Values                 Description
============================= =================================================

Globals        Type           Description
============== ============== =================================================

Locals         Type           Description
============== ============== =================================================

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
