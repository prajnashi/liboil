/* downsample2 f64 functions
 * Copyright (C) 2001,2002  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
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

