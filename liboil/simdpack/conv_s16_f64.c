/* convert s16 to f64
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
Kernel: conv_s16_f64
Description: convert s16 to f64

Converts s16 to f64.
*/

#ifndef _conv_s16_f64_h_
#define _conv_s16_f64_h_

#include <math.h>

#include <sl_types.h>


/* storage class */
#ifndef SL_conv_s16_f64_storage
 #ifdef SL_storage
  #define SL_conv_s16_f64_storage SL_storage
 #else
  #define SL_conv_s16_f64_storage static inline
 #endif
#endif


/* IMPL conv_s16_f64_ref */
SL_conv_s16_f64_storage
void conv_s16_f64_ref(f64 *dest, s16 *src, int n)
{
	int i;
	for(i=0;i<n;i++){
		dest[i]=src[i];
	}
}


/* Table lookup.  Could be useful on some platforms, but the direct
 * bit stuffing function is probably faster. */
/* IMPL conv_s16_f64_table */
SL_conv_s16_f64_storage
void conv_s16_f64_table(f64 *dest, s16 *src, int n)
{
	static f64 ints_high[256];
	static f64 ints_low[256];
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

	SL_UNROLL4(
		idx = (unsigned short)*src++;
		*dest++ = ints_high[(idx>>8)] + ints_low[(idx&0xff)];
	,n)
}


/* IMPL conv_s16_f64_unroll */
SL_conv_s16_f64_storage
void conv_s16_f64_unroll(f64 *dest, s16 *src, int n)
{
	if(n&1){
		*dest++ = *src++;
		n--;
	}
	if(n&2){
		*dest++ = *src++;
		*dest++ = *src++;
		n-=2;
	}
	while(n>0){
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		*dest++ = *src++;
		n-=4;
	}
}


/* stuff bits directly into the double.  This works for the entire
 * input range. */
#ifdef HAVE_IEEE754_H 
#include <ieee754.h>

/* IMPL conv_s16_f64_bits defined(HAVE_IEEE754_H) */
SL_conv_s16_f64_storage
void conv_s16_f64_bits(f64 *dest, s16 *src, int n)
{
	int i;
	union ieee754_double id;
	f64 offset = 98304.0;

	id.d = 0x1ffff;
	for(i=0;i<n;i++){
		id.ieee.mantissa0 = (src[i]^0x8000)<<4;
		dest[i] = id.d - offset;
	}
}
#endif


#ifdef __powerpc__
/* Altivec has a special opcode to convert int32's to floats.  It's
 * not exactly what we want here, but it will do.  This could easily
 * be improved by doing the short->int conversion in altivec, and
 * multiplex the float->double conversion in with the altivec
 * statements. */
/* IMPL conv_s16_f64_altivec defined(__powerpc__) */
SL_conv_s16_f64_storage
void conv_s16_f64_altivec(f64 *dest, s16 *src, int n)
{
	static union { int i[4]; float f[4]; } av_tmp
		__attribute__ ((__aligned__ (16)));
	int i;

	if(n&1){
		*dest++ = *src++;
		n--;
	}
	if(n&2){
		*dest++ = *src++;
		*dest++ = *src++;
		n-=2;
	}
	for(i=0;i<n;i+=4){
		av_tmp.i[0] = src[0];
		av_tmp.i[1] = src[1];
		av_tmp.i[2] = src[2];
		av_tmp.i[3] = src[3];

		asm(
		"lvx 0,0,%0\n"
		"\tvcfsx 1,0,0\n"
		"\tstvx 1,0,%0\n"
		: : "r" (&av_tmp)
		);

		dest[0]=av_tmp.f[0];
		dest[1]=av_tmp.f[1];
		dest[2]=av_tmp.f[2];
		dest[3]=av_tmp.f[3];
		src += 4;
		dest += 4;
	}
}
#endif

#endif


#ifdef TEST_conv_s16_f64
int TEST_conv_s16_f64(void)
{
	int i;
	int pass;
	int failures=0;
	s16 *src;
	f64 *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src = sl_malloc_s16(N);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(conv_s16_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src[i] = sl_rand_s16();
		}

		conv_s16_f64_ref(dest_ref,src,N);
		sl_profile_start(t);
		conv_s16_f64_FUNC(dest_test,src,N);	
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %d %g %g\n",i,src[i],dest_ref[i],
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

