/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2001,2003  David A. Schleef <ds@schleef.org>
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
#include <liboil.h>
#include <conv.h>

#include <math.h>


static void
conv_f64_s16_table(double *dest, int dest_stride, short *src,
	int src_stride, int n)
{
	static float ints_high[256];
	static float ints_low[256];
	static int init = 0;
	int i;
	unsigned int idx;
	if(!init){
		for(i=0;i<256;i++){
			ints_high[i]=256.0*((i<128)?i:i-256);
			ints_low[i]=i;
		}
		init = 1;
	}

	if(n&1){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		n-=1;
	}
	for(i=0;i<n;i+=2){
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
		idx = (unsigned short)*src;
		*dest = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
		OIL_INCREMENT(dest, dest_stride);
		OIL_INCREMENT(src, src_stride);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_table, conv_f64_s16);


#ifdef HAVE_CPU_PPC
void conv_f64_s16_altivec(double *dest, int dest_stride, short *src,
	int src_stride, int n)
{
	static union { int i[4]; float f[4]; } av_tmp
		__attribute__ ((__aligned__ (16)));
	int i;

	for(i=0;i<n;i+=4){
		av_tmp.i[0] = *OIL_OFFSET(src,src_stride * 0);
		av_tmp.i[1] = *OIL_OFFSET(src,src_stride * 1);
		av_tmp.i[2] = *OIL_OFFSET(src,src_stride * 2);
		av_tmp.i[3] = *OIL_OFFSET(src,src_stride * 3);

		asm(
		"	lvx 0,0,%0\n"
		"	vcfsx 1,0,0\n"
		"	stvx 1,0,%0\n"
		: : "r" (&av_tmp)
		);

		*OIL_OFFSET(dest,dest_stride * 0) =av_tmp.f[0];
		*OIL_OFFSET(dest,dest_stride * 1) =av_tmp.f[1];
		*OIL_OFFSET(dest,dest_stride * 2) =av_tmp.f[2];
		*OIL_OFFSET(dest,dest_stride * 3) =av_tmp.f[3];
		src = OIL_OFFSET(src, src_stride * 4);
		dest = OIL_OFFSET(dest, dest_stride * 4);
	}
}
OIL_DEFINE_IMPL(conv_f64_s16_altivec, conv_f64_s16);
#endif


/* double to short */

#ifdef HAVE_CPU_PPC
void clipconv_s16_f64_ppcasm(short *dest, double *src, int n)
{
	int tmp[2];
	double min = -32768.0;
	double max = 32767.0;
	double ftmp0, ftmp1;

	asm __volatile__(
	"	addic. %3,%3,-8		\n"
	"	addic. %6,%6,-2		\n"
	"loop:				\n"
	"	lfdu %0,8(%3)		\n"
	"	fsub %1,%0,%4		\n"
	"	fsel %0,%1,%0,%4	\n"
	"	fsub %1,%0,%5		\n"
	"	fsel %0,%1,%5,%0	\n"
	"	fctiw %1,%0		\n"
	"	addic. 5,5,-1		\n"
	"	stfd %1,0(%2)		\n"
	"	lhz 9,6(%2)		\n"
	"	sthu 9,2(%6)		\n"
	"	bne loop		\n"
	: "=&f" (ftmp0),
	  "=&f" (ftmp1)
	: "b" (tmp),
	  "r" (src),
	  "f" (min),
	  "f" (max),
	  "r" (dest)
	: "r9",
	  "r5" );
}
OIL_DEFINE_IMPL(clipconv_s16_f64_ppcasm, clipconv_s16_f64);
#endif


