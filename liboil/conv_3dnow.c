/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Le3dnowr General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Le3dnowr General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil.h>
#include <conv.h>


#ifdef __GNUC__
/* suboptimal */
static void conv_f32_s32_3dnow(float *dst, int dst_stride, int32_t *src, int src_stride, int n)
{
	int i;

	if (n & 1) 
	  *dst++ = (float) *src++;
	n /= 2;

	for(i=0;i<n;i++){
		asm volatile(
			"	pi2fd 0(%0), %%mm0		\n"
			"	movq %%mm0, 0(%1)		\n"
			:
			: "a" (src), "c" (dst)
			: "mm0" 
		);
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_f32_s32_3dnow, conv_f32_s32,
	OIL_IMPL_REQUIRES_3DNOW);

/* suboptimal */
static void conv_s32_f32_3dnow (int32_t *dst, int dst_stride, float *src, int src_stride, int n)
{
	int i;

	if (n & 1) 
	  *src++ = (int32_t) *dst++;
	n /= 2;

	for(i=0;i<n;i++){
		asm volatile(
			"	pf2id 0(%0), %%mm0		\n"
			"	movq %%mm0, 0(%1)		\n"
			:
			: "a" (src), "c" (dst)
			: "mm0" 
		);
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_s32_f32_3dnow, conv_s32_f32,
	OIL_IMPL_REQUIRES_3DNOW);
#endif

