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
Kernel: downsamplex_f64
Description: downsamples a scan line using linear interpolation

This is an excessively mathematical way of describing linear
interpolation, but it's precise:

  Let f(x) be the function where f(x) = src[floor(x)].  Let
  ratio = nsrc/ndest.  Then the output
  dest[i] = Integrate[f(x),{x,i*ratio,(i+1)*ratio}]/ratio.

Downsampling is only valid when 1.0 < ratio < 2.0.  Note that this
is "<" and not "<=".  If you want to downsample further, you should
either downsample to a power of 2, and then use downsample2, or use
downsample2 and then downsample.  The former will probably give
better results, but slower.

This function will probably be changed, as usage patterns develop.
*/

#ifndef _downsamplex_f64_h_
#define _downsamplex_f64_h_

#include <math.h>
#include <sl_types.h>


/* storage class */
#ifndef SL_downsamplex_f64_storage
 #ifdef SL_storage
  #define SL_downsamplex_f64_storage SL_storage
 #else
  #define SL_downsamplex_f64_storage static inline
 #endif
#endif


/* IMPL downsamplex_f64_ref */
SL_downsamplex_f64_storage
void downsamplex_f64_ref(f64 *dest, f64 *src, int ndest, int nsrc, f64 ratio,
	f64 invratio)
{
	int i,j;
	double x;

#if 0
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (j+1 - i*ratio) * src[j];
		if(j+2 < floor((i+1)*ratio)){
			x += src[j+1];
			x += ((i+1)*ratio - (j+2)) * src[j+2];
		}else{
			x += ((i+1)*ratio - (j+1)) * src[j+1];
		}
		dest[i] = x * invratio;
	}
#endif
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (i - j*invratio)*src[j];
		x += ((j+1)*invratio - i)*src[j+1];
		dest[i] = x * ratio;
	}
}

/* IMPL downsamplex_f64_fast */
SL_downsamplex_f64_storage
void downsamplex_f64_fast(f64 *dest, f64 *src, int ndest, int nsrc, f64 ratio,
	f64 invratio)
{
	int i,j;
	double x;
	unsigned int step;
	unsigned int acc;

	acc = 0;
	j = 0;
	step = (ratio-1.0)*(1<<16);
	for(i=0;i<ndest;i++){
//		printf("# %d %d %d %g\n",i,j,acc,floor(i*ratio));
		x = (j+1 - i*ratio) * src[j];
		x += ((i+1)*ratio - (j+1)) * src[j+1];
		dest[i] = x * invratio;
		acc += step;
		j += 1 + (acc>>16);
		acc &= (1<<16)-1;
	}
}

#endif

