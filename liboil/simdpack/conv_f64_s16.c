/* convert f64 to s16 functions
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
Kernel: conv_f64_s16
Description: round source value to nearest integer and convert to s16.

Result is undefined if src<-32768 or src>32767.
Half integers may be rounded to either nearby integer.
*/

#ifndef _conv_f64_s16_h_
#define _conv_f64_s16_h_

#include <math.h>
#include <sl_types.h>


/* storage class */
#ifndef SL_conv_f64_s16_storage
 #ifdef SL_storage
  #define SL_conv_f64_s16_storage SL_storage
 #else
  #define SL_conv_f64_s16_storage static inline
 #endif
#endif


/*
 * The reference implementation does a round-to-lower, other
 * implementations might be different.
 */

/* IMPL conv_f64_s16_ref */
SL_conv_f64_s16_storage
void conv_f64_s16_ref(s16 *dest, f64 *src, int n)
{
	int i;

	for(i=0;i<n;i++){
		dest[i] = floor(src[i]+0.5);
	}
}


SL_conv_f64_s16_storage
void conv_f64_s16_dstr_sstr(s16 *dest, f64 *src,
	int n, int dstr, int sstr)
{
	void *d = dest;
	void *s = src;

	SL_UNROLL4(
		*(s16 *)d = floor(*(f64 *)s + 0.5);
		d += dstr; s += sstr;
	,n);
}

/* IMPL conv_f64_s16_unroll */
SL_conv_f64_s16_storage
void conv_f64_s16_unroll(s16 *dest, f64 *src, int n)
{
	conv_f64_s16_dstr_sstr(dest,src,n,sizeof(s16),sizeof(f64));
}

#ifdef HAVE_IEEE754_H
#include <ieee754.h>
/* IMPL conv_f64_s16_bits defined(HAVE_IEEE754_H) */
SL_conv_f64_s16_storage
void conv_f64_s16_bits(s16 *dest, f64 *src,
	int n)
{
	int i;
	union ieee754_double id;
	double offset = 98304.5;

	for(i=0;i<n;i++){
		id.d = offset + src[i];
		dest[i] = 0x8000 ^ (id.ieee.mantissa0 >> 4);
	}
}
#endif

#ifdef HAVE_CPU_POWERPC
SL_conv_f64_s16_storage
void conv_f64_s16_ppcasm(s16 *dest, f64 *src, int n)
{
	f64 ftmp;

	dest--;
	src--;
	asm __volatile__(
		"	mtctr %2\n"
		"1:	lfdu 0,8(%1)\n"
		"	fctiw 1,0\n"
		"	stfd 1,0(%3)\n"
		"	lhz 11,6(%3)\n"
		"	sthu 11,2(%0)\n"
		"	bdnz 1b\n"
	: "+b" (dest), "+b" (src)
	: "b" (n), "b" (&ftmp)
	: "32", "33", "11", "ctr" );
}
#endif

#ifdef HAVE_CPU_I386
SL_conv_f64_s16_storage
void conv_f64_s16_i386asm(s16 *dest, f64 *src,
	int n)
{
	int i;

	for(i=0;i<n;i++){
		asm(
		"	fldl (%0)\n"
		"	fistpl (%1)\n"
		: : "r" (src), "r" (dest) : "memory");
		dest++;
		src++;
	}
}
#endif

#endif

#ifdef TEST_conv_f64_s16
int TEST_conv_f64_s16(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src;
	s16 *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_s16(N);
	dest_test = sl_malloc_s16(N);

	sl_profile_init(t);
	srand(20020303);

	printf("I: " sl_stringify(conv_f64_s16_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src[i] = sl_rand_f64_s16();
		}
		conv_f64_s16_ref(dest_ref,src,N);
		sl_profile_start(t);
		conv_f64_s16_FUNC(dest_test,src,N);	
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %d %d\n",i,src[i],dest_ref[i],
					dest_test[i]);
				failures++;
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

