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

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>

#include <math.h>


OIL_DEFINE_CLASS (mult8x8_s16, 
    "int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1, int sstr2");

static void
mult8x8_s16_ref(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1,
    int sstr2)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			OIL_GET(dest,dstr*i+j, int16_t) =
				OIL_GET(src1,sstr1*i+j, int16_t) *
				OIL_GET(src1,sstr1*i+j, int16_t);
		}
	}
}

OIL_DEFINE_IMPL_REF (mult8x8_s16_ref, mult8x8_s16);

