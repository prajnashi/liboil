/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003 David A. Schleef <ds@schleef.org>
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
#include <liboil/simdpack/simdpack.h>
#include <math.h>

OIL_DEFINE_CLASS (downsample1x_f64,
    "double *dest, int dstr, double *src, int sstr, "
    "int ndest, int nsrc, double param1, double param2");

/*
Kernel: downsample1x_f64
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

static void
downsample1x_f64_ref (double *dest, int dstr, double *src, int sstr,
    int ndest, int nsrc, double ratio, double invratio)
{
	int i,j;
	//double x;

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
		j = floor((0.49999995+i)*ratio);
		dest[i] = src[j];
	}
#if 0
	for(i=0;i<ndest;i++){
		j = floor(i*ratio);
		x = (i - j*invratio)*src[j];
		x += ((j+1)*invratio - i)*src[j+1];
		dest[i] = x * ratio;
	}
#endif
}

OIL_DEFINE_IMPL_REF (downsample1x_f64_ref, downsample1x_f64_class);

static void
downsample1x_f64_fast (double *dest, int dstr, double *src, int sstr,
    int ndest, int nsrc, double ratio, double invratio)
{
	int i,j;
	double x;
#if 0
	unsigned int step;
	unsigned int acc;

	j = 0;
	step = (ratio-1.0)*((1<<31)+1);
	//acc = (1<<30) - 1;
	acc = step>>1;
	for(i=0;i<ndest;i++){
//		printf("# %d %d %d %g\n",i,j,acc,floor(i*ratio));
		dest[i] = src[j];
		acc += step;
		j += 1 + (acc>>31);
		acc &= (1<<31)-1;
	}
#else
	unsigned int step;
	unsigned int acc;
	int k;
	const unsigned int fac = 1<<16;

	x = 0.5 * ratio;
	acc = fac * x;
	step = fac * (ratio - 1.0);
	k = 0;
	for(i=0;i<ndest;i++){
		j = floor(x);
		//dest[i] = src[j];
		dest[i] = src[k];

		x += ratio;
		acc += step;
		k++;
		if(acc>fac){
			k++;
			acc -= fac;
		}
		//while(x>=1){ x-=1.0; k++; }
	}
#endif
}

OIL_DEFINE_IMPL (downsample1x_f64_fast, downsample1x_f64_class);

