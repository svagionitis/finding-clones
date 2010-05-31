#ifndef __PNM_H__
#define __PNM_H__


unsigned char *load_ppm(const char *filename, int *width, int *height);
unsigned char *load_pgm(const char *filename, int *width, int *height);

#endif
