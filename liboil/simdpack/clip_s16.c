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

#if 0
static void
clip_s16_ref (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	int i;
	int16_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr, int16_t);
		if(x>hi) x = hi;
		if(x<low) x = low;
		OIL_GET(dest,i*dstr, int16_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s16_ref, clip_s16);
#endif

/* This is a suprisingly fast implementation of clipping
 * in straight C.  It would be difficult to do it faster in asm
 * without specialized opcodes. */

static void
clip_s16_fast (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	int i;
	int32_t x;

	for(i=0;i<n;i++){
		x = OIL_GET(src,i*sstr, int16_t);
		x = x - (((x-low)>>31)&(x-low)) + (((hi-x)>>31)&(hi-x));
		OIL_GET(dest,i*dstr, int16_t) = x;
	}
}

OIL_DEFINE_IMPL (clip_s16_fast, clip_s16);

static void
clip_s16_fast2 (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	int32_t x1, x2;

	n/=2;
	while(n>0){
		x1 = *src;
		x1 = *OIL_INCREMENT(src,sstr);
		 x2 = *src;
		 x2 = *OIL_INCREMENT(src,sstr);
		x1 -= (((x1-low)>>31)&(x1-low));
		 x2 -= (((x2-low)>>31)&(x2-low));
		*dest = x1 + (((hi-x1)>>31)&(hi-x1));
		*OIL_INCREMENT(dest,dstr);
		 *dest = x2 + (((hi-x2)>>31)&(hi-x2));
		 *OIL_INCREMENT(dest,dstr);
		n--;
	}
}
OIL_DEFINE_IMPL (clip_s16_fast2, clip_s16);

#ifdef HAVE_CPU_POWERPC
static void
clip_s16_ppcasm (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	low=-low;
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
	: "+b" (dest), "+b" (src), "+b" (low), "+b" (hi), "+b" (n)
	: 
	: "7", "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm, clip_s16);
#endif

#ifdef HAVE_CPU_POWERPC
SL_clip_s16_storage
clip_s16_ppcasm2 (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	low=-low;
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
	: "+b" (dest), "+b" (src), "+b" (low), "+b" (hi), "+b" (n)
	: 
	: "9", "10", "8", "11", "0", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm2, clip_s16);
#endif

#ifdef HAVE_CPU_POWERPC
/* This is just a basic weave of the previous function.  It uses
 * a lot of registers and gets pretty hairy, so it would take some
 * work to make better. */
SL_clip_int16_t_storage
clip_s16_ppcasm3 (int16_t *dest, int dstr, int16_t *src, int sstr, int16_t low,
    int16_t hi, int n)
{
	low=-low;
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
		"	 srawi 30,20,31		\n"
		"	and 0,0,10		\n"
		"	 srawi 21,18,31		\n"
		"	and 11,11,8		\n"
		"	 and 30,30,20		\n"
		"	subf 9,0,9		\n"
		"	 and 21,21,18		\n"
		"	add 9,9,11		\n"
		"	 subf 19,30,19		\n"
		"	sthu 9,2(%0)		\n"
		"	 add 19,19,21		\n"
		"	 sthu 19,2(%0)		\n"
		"	bdnz 1b			\n"
	: "+b" (dest), "+b" (src), "+b" (low), "+b" (hi), "+b" (n)
	: 
	: "8", "9", "10", "11", "0",
	  "18", "19", "20", "21", "30", "ctr");
}
OIL_DEFINE_IMPL (clip_s16_ppcasm3, clip_s16);
#endif


#ifdef TEST_clip_int16_t
int TEST_clip_int16_t(void)
{
	int i;
	int failures = 0;
	int pass;
	int16_t *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_int16_t(N);
	dest_ref = sl_malloc_int16_t(N);
	dest_test = sl_malloc_int16_t(N);

	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(clip_int16_t_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++)src[i]=sl_rand_int16_t();

		clip_int16_t_ref(dest_ref,src,0xff80,0x007f,N);
		sl_profile_start(t);
		clip_int16_t_FUNC(dest_test,src,0xff80,0x007f,N);
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %d %d %d\n",i,src[i],dest_ref[i],
					dest_test[i]);
			}
		}
	}

	sl_free(src);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

