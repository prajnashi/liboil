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

OIL_DEFINE_CLASS_X (dequantize8x8_s16, 
    "int16_t *dest, int dstr, int16_t *src1, int sstr1, int16_t *src2, int sstr2");

#define BLOCK8x8_S16(ptr, stride, row, column) \
	(*((int16_t *)((void *)ptr + stride*row) + column))

static void
dequantize8x8_s16_ref (int16_t *dest, int dstr, int16_t *src, int sstr,
    int16_t *quant, int qstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			BLOCK8x8_S16(dest,dstr,i,j) =
				BLOCK8x8_S16(src,sstr,i,j) *
				BLOCK8x8_S16(quant,qstr,i,j);
		}
	}
}

OIL_DEFINE_IMPL_REF (dequantize8x8_s16_ref, dequantize8x8_s16_class);

