/* downsample2 u8 functions
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
Kernel: downsample2_u8
Description: downsamples a scan line by a factor of 2 by averaging

Averages sucessive input samples to produce output samples.

*/

#include <math.h>
#include <sl_types.h>
#include <endian.h>


static void
downsample2_u8_ref(u8 *dest, u8 *src, int n)
{
	int i;

	for(i=0;i<n/2;i++){
		dest[i] = (src[i*2] + src[i*2+1])>>1;
	}
}

#include <average2_u8.h>
static void
downsample2_u8_ave(u8 *dest, u8 *src, int n)
{
	average2_u8(dest,src,2,src+1,2,n/2);
}

static void
downsample2_u8_fast(u8 *dest, u8 *src, int n)
{
	while(n>1){
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		n-=2;
	}
}

static void
downsample2_u8_trick1(u8 *dest, u8 *src, int n)
{
	unsigned int x, y;

	while(n>1){
#if __BYTE_ORDER == __LITTLE_ENDIAN
		x = *src++;
		y = *src++;
		x |= (*src++)<<8;
		y |= (*src++)<<8;
		x |= (*src++)<<16;
		y |= (*src++)<<16;
		x |= (*src++)<<24;
		y |= (*src++)<<24;
#else
		x = *src++;
		y = *src++;
		x = (x<<8) | *src++;
		y = (y<<8) | *src++;
		x = (x<<8) | *src++;
		y = (y<<8) | *src++;
		x = (x<<8) | *src++;
		y = (y<<8) | *src++;
#endif
		*(unsigned int *)dest = (((x^y)&0xfefefefe)>>1) + (x&y);
		dest += 4;
		n-=8;
	}
}

static void
downsample2_u8_trick2(u8 *dest, u8 *src, int n)
{
	unsigned int x, y;

	while(n>1){
#if __BYTE_ORDER == __LITTLE_ENDIAN
		x = (src[6]<<24) | (src[4]<<16) | (src[2]<<8) | src[0];
		y = (src[7]<<24) | (src[5]<<16) | (src[3]<<8) | src[1];
#else
		x = (src[0]<<24) | (src[2]<<16) | (src[4]<<8) | src[6];
		y = (src[1]<<24) | (src[3]<<16) | (src[5]<<8) | src[7];
#endif
		src+=8;
		*(unsigned int *)dest = (((x^y)&0xfefefefe)>>1) + (x&y);
		dest += 4;
		n-=8;
	}
}

static void
downsample2_u8_trick3(u8 *dest, u8 *src, int n)
{
	unsigned int x, y;

	while(n>1){
		x = *(unsigned int *)src;	/* 0123 */
		y = x>>8;			/*  012 */
		src+=4;
		x = (((x^y)&0xfefefefe)>>1) + (x&y);
#if __BYTE_ORDER == __LITTLE_ENDIAN
		*dest++ = x;
		*dest++ = x>>16;
#else
		*dest++ = x>>16;
		*dest++ = x;
#endif
		n-=4;
	}
}

static void
downsample2_u8_unroll4(u8 *dest, u8 *src, int n)
{
	while(n&0x6){
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		n-=2;
	}
	while(n>1){
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		*dest++ = (src[0] + src[1])>>1;
		src+=2;
		n-=8;
	}
}

#ifdef HAVE_CPU_POWERPC
static void
downsample2_u8_ppcasm(u8 *dest, u8 *src, int n)
{
	dest--;
	src--;
	n/=2;
	asm __volatile__(
		"	mtctr %2		\n"
		"1:	lbz	0, 1(%1) 	\n"
		"  	lbzu	9, 2(%1) 	\n"
		"  	add	9, 0, 9 	\n"
		"  	srawi	0, 9, 1		\n"
		"  	stbu	0, 1(%0)	\n"
		"	bdnz	1b		\n"
	: "+b" (dest), "+b" (src)
	: "b" (n)
	: "0", "9", "ctr" );

}
#endif


