/* downsample2 s16 functions
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


