/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2002 David A. Schleef <ds@schleef.org>
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

/*
Kernel: downsample2_s16
Description: downsamples a scan line by a factor of 2 by averaging

Averages sucessive input samples to produce output samples.

*/

#include <math.h>
#include <sl_types.h>

static void
downsample2_s16_ref(s16 *dest, s16 *src, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		dest[i] = (src[i*2] + src[i*2+1] + 1)>>1;
	}
}

static void
downsample2_s16_fast(s16 *dest, s16 *src, int n)
{
	while(n>1){
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		n-=2;
	}
}

static void
downsample2_s16_unroll4(s16 *dest, s16 *src, int n)
{
	while(n&0x6){
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		n-=2;
	}
	while(n>1){
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		*dest++ = (src[0] + src[1] + 1)>>1;
		src+=2;
		n-=8;
	}
}

#ifdef HAVE_CPU_POWERPC
static void
downsample2_s16_ppcasm(s16 *dest, s16 *src, int n)
{
	dest--;
	src--;
	n/=2;
	asm __volatile__(
		"	mtctr %2		\n"
		"1:	lhau	0, 2(%1) 	\n"
		"  	lhau	9, 2(%1) 	\n"
		"  	add	9, 0, 9 	\n"
		"  	addi	9, 9, 1 	\n"
		"  	srawi	0, 9, 1		\n"
		"  	sthu	0, 2(%0)	\n"
		"	bdnz	1b		\n"
	: "+b" (dest), "+b" (src)
	: "b" (n)
	: "0", "9", "ctr" );

}
#endif


