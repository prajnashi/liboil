/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboil.h>
#include "jpeg.h"


#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((void *)ptr + stride*row) + column))

#if 0
static const unsigned char zigzag_order[64] = {
	0,
	8, 1,
	2, 9, 16,
	24, 17, 10, 3, 
	4, 11, 18, 25, 32,
	40, 33, 26, 19, 12, 5, 
	6, 13, 20, 27, 34, 41, 48,
	56, 49, 42, 35, 28, 21, 14, 7,
	15, 22, 29, 36, 43, 50, 57,
	58, 51, 44, 37, 30, 23,
	31, 38, 45, 52, 59,
	60, 53, 46, 39,
	47, 54, 61,
	62, 55,
	63
};
#endif

static const unsigned char unzigzag_order[64] = {
	0,  1,  5,  6,  14, 15, 27, 28,
	2,  4,  7,  13, 16, 26, 29, 42,
	3,  8,  12, 17, 25, 30, 41, 43,
	9,  11, 18, 24, 31, 40, 44, 53,
	10, 19, 23, 32, 39, 45, 52, 54,
	20, 22, 33, 38, 46, 51, 55, 60,
	21, 34, 37, 47, 50, 56, 59, 61,
	35, 36, 48, 49, 57, 58, 62, 63,
};


static void
unzigzag8x8_s16_c(int16_t *dest, int dstr, int16_t *src, int sstr)
{
	int i,j;
	unsigned int z;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			z = unzigzag_order[i*8+j];
			BLOCK8x8_S16(dest,dstr,i,j) =
				BLOCK8x8_S16(src,sstr,(z>>3),(z&7));
		}
	}
}

OIL_DEFINE_IMPL (unzigzag8x8_s16_c, unzigzag8x8_s16_class);

