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
Kernel: downsample2_f64
Description: downsamples a scan line by a factor of 2 by averaging

Averages sucessive input samples to produce output samples.

*/

#ifndef _downsample2_f64_h_
#define _downsample2_f64_h_

#include <math.h>
#include <sl_types.h>


/* storage class */
#ifndef SL_downsample2_f64_storage
 #ifdef SL_storage
  #define SL_downsample2_f64_storage SL_storage
 #else
  #define SL_downsample2_f64_storage static inline
 #endif
#endif


/* IMPL downsample2_f64_ref */
SL_downsample2_f64_storage
void downsample2_f64_ref(f64 *dest, f64 *src, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		dest[i] = (src[i*2] + src[i*2+1])/2;
	}
}

/* IMPL downsample2_f64_fast */
SL_downsample2_f64_storage
void downsample2_f64_fast(f64 *dest, f64 *src, int n)
{
	while(n>1){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=2;
	}
}

/* IMPL downsample2_f64_unroll4 */
SL_downsample2_f64_storage
void downsample2_f64_unroll4(f64 *dest, f64 *src, int n)
{
	while(n&0x6){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=2;
	}
	while(n>1){
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		*dest++ = 0.5*(src[0] + src[1]);
		src+=2;
		n-=8;
	}
}

#endif

