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

/* This is a suprisingly fast implementation of clipping
 * in straight C.  It would be difficult to do it faster in asm
 * without specialized opcodes. */

static void
clip_s16_fast (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int i;
	int32_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr, int16_t);
		x = x - (((x-*low)>>31)&(x-*low)) + (((*hi-x)>>31)&(*hi-x));
		OIL_GET(dest,i*dstr, int16_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s16_fast, clip_s16);

static void
clip_s16_fast2 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int32_t x1, x2;

        if (n&1) {
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
        }
	while(n>0){
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		 x2 = *src;
		 OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		 x2 -= (((x2-*low)>>31)&(x2-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
		 *dest = x2 + (((*hi-x2)>>31)&(*hi-x2));
		 OIL_INCREMENT(dest,dstr);
		n-=2;
	}
}
OIL_DEFINE_IMPL (clip_s16_fast2, clip_s16);

