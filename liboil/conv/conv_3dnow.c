/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <liboil/liboilfunction.h>
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
        asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL(conv_f32_s32_3dnow, conv_f32_s32,
	OIL_IMPL_FLAG_3DNOW);

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
        asm volatile ("emms");
}
OIL_DEFINE_IMPL_FULL(conv_s32_f32_3dnow, conv_s32_f32,
	OIL_IMPL_FLAG_3DNOW);
#endif

