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


