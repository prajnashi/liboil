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
#include <conv.h>


#ifdef __GNUC__
/* suboptimal */
static void conv_f64_s32_sse(double *dst, int dst_stride, int32_t *src, int src_stride, int n)
{
	int i;

	for(i=0;i<n;i++){
		asm volatile(
			"	cvtsi2sd 0(%0), %%xmm0		\n"
			"	movq %%xmm0, 0(%1)		\n"
			:
			: "a" (src), "c" (dst)
			/* : "xmm0" */
		);
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_f64_s32_sse, conv_f64_s32,
	OIL_IMPL_REQUIRES_SSE2);

/* suboptimal */
static void conv_s32_f64_sse(int32_t *dst, int dst_stride, double *src, int src_stride, int n)
{
	int i;
	int eax;

	for(i=0;i<n;i++){
		asm volatile(
			"	cvtsd2si 0(%1), %0		\n"
			: "=a" (eax)
			: "c" (src)
		);
		*dst = eax;
		dst = OIL_OFFSET(dst, dst_stride);
		src = OIL_OFFSET(src, src_stride);
	}
}
OIL_DEFINE_IMPL_FULL(conv_s32_f64_sse, conv_s32_f64,
	OIL_IMPL_REQUIRES_SSE2);
#endif

