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
#include <liboil/simdpack/simdpack.h>

/* This is a suprisingly fast implementation of clipping
 * in straight C.  It would be difficult to do it faster in asm
 * without specialized opcodes. */

static void
clip_s16_fast (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int i;
	int32_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr, int16_t);
		x = x - (((x-*low)>>31)&(x-*low)) + (((*hi-x)>>31)&(*hi-x));
		OIL_GET(dest,i*dstr, int16_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s16_fast, clip_s16);

static void
clip_s16_fast2 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	int32_t x1, x2;

        if (n&1) {
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
        }
	while(n>0){
		x1 = *src;
		OIL_INCREMENT(src,sstr);
		 x2 = *src;
		 OIL_INCREMENT(src,sstr);
		x1 -= (((x1-*low)>>31)&(x1-*low));
		 x2 -= (((x2-*low)>>31)&(x2-*low));
		*dest = x1 + (((*hi-x1)>>31)&(*hi-x1));
		OIL_INCREMENT(dest,dstr);
		 *dest = x2 + (((*hi-x2)>>31)&(*hi-x2));
		 OIL_INCREMENT(dest,dstr);
		n-=2;
	}
}
OIL_DEFINE_IMPL (clip_s16_fast2, clip_s16);

#ifdef HAVE_CPU_POWERPC
static void
clip_s16_ppcasm (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	__asm__ __volatile__(
		"	li 7, 0			\n"
		"	mtctr %4		\n"
		"1:	lhax 9,7,%1		\n"
		"	add 10,9,%2		\n"
		"	subfc 8,9,%3		\n"
		"	srawi 0,10,31		\n"
		"	srawi 11,8,31		\n"
		"	and 0,0,10		\n"
		"	and 11,11,8		\n"
		"	subf 9,0,9		\n"
		"	add 9,9,11		\n"
		"	sthx 9,7,%0		\n"
		"	addi 7,7,2		\n"
		"	bdnz 1b			\n"
        :
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "7", "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm, clip_s16);
#endif

#ifdef HAVE_CPU_POWERPC
static void
clip_s16_ppcasm2 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	src--;
	dest--;
	__asm__ __volatile__(
		"	mtctr %4		\n"
		"1:	lhau 9,2(%1)		\n"
		"	add 10,9,%2		\n"
		"	subfc 8,9,%3		\n"
		"	srawi 0,10,31		\n"
		"	srawi 11,8,31		\n"
		"	and 0,0,10		\n"
		"	and 11,11,8		\n"
		"	subf 9,0,9		\n"
		"	add 9,9,11		\n"
		"	sthu 9,2(%0)		\n"
		"	bdnz 1b			\n"
        :
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm2, clip_s16);
#endif

#ifdef HAVE_CPU_POWERPC
/* This is just a basic weave of the previous function.  It uses
 * a lot of registers and gets pretty hairy, so it would take some
 * work to make better. */
static void
clip_s16_ppcasm3 (int16_t *dest, int dstr, int16_t *src, int sstr, int n,
    int16_t *low, int16_t *hi)
{
	src--;
	dest--;
	n/=2;
	__asm__ __volatile__("\n"
		"	mtctr %4		\n"
		"1:	lhau 9,2(%1)		\n"
		"	add 10,9,%2		\n"
		"	 lhau 19,2(%1)		\n"
		"	subfc 8,9,%3		\n"
		"	 add 20,19,%2		\n"
		"	srawi 0,10,31		\n"
		"	 subfc 18,19,%3		\n"
		"	srawi 11,8,31		\n"
		"	 srawi 22,20,31		\n"
		"	and 0,0,10		\n"
		"	 srawi 21,18,31		\n"
		"	and 11,11,8		\n"
		"	 and 22,22,20		\n"
		"	subf 9,0,9		\n"
		"	 and 21,21,18		\n"
		"	add 9,9,11		\n"
		"	 subf 19,22,19		\n"
		"	sthu 9,2(%0)		\n"
		"	 add 19,19,21		\n"
		"	 sthu 19,2(%0)		\n"
		"	bdnz 1b			\n"
	: 
	: "b" (dest), "b" (src), "b" (-*low), "b" (*hi), "b" (n)
	: "8", "9", "10", "11", "0",
	  "18", "19", "20", "21", "22", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm3, clip_s16);
#endif


