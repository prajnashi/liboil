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
 * without specialized opcodes.  However, this trick clips
 * the range min^(1<<31) to max^(1<<31) incorrectly.  So
 * it's limited to 31 bits. */

static void
clip_s32_fast (int32_t *dest, int dstr, int32_t *src, int sstr, int n,
    int32_t *low, int32_t *hi)
{
	int i;
	int32_t x;

	for(i=0;i<n;i++){
		x = src[i];
		dest[i] = x - (((x-*low)>>31)&(x-*low)) + (((*hi-x)>>31)&(*hi-x));
	}
}

OIL_DEFINE_IMPL (clip_s32_fast, clip_s32);


