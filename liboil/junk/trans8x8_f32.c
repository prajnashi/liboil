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
Kernel: trans8x8_f32
Description: transpose 8x8 block

XXX
*/

#ifndef _trans8x8_f32_h_
#define _trans8x8_f32_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>
#include <sl_altivec.h>

/* storage class */
#ifndef SL_trans8x8_f32_storage
 #ifdef SL_storage
  #define SL_trans8x8_f32_storage SL_storage
 #else
  #define SL_trans8x8_f32_storage static inline
 #endif
#endif


/* IMPL trans8x8_f32_ref */
SL_trans8x8_f32_storage
void trans8x8_f32_ref(f32 *dest, f32 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_f32(dest,dstr,i,j) =
				block8x8_f32(src,sstr,j,i);
		}
	}
}


#ifdef SIMDPACK_USE_ALTIVEC
/* should be moved */
SL_trans8x8_f32_storage
void trans4x4_f32_a16_altivec(f32 *dest, f32 *src, int dstr, int sstr)
{
	/* load vectors */
	sl_altivec_load4_0(src,sstr);

	/* transpose 4x4 */
	__asm__ __volatile__(
		"vmrghw 4, 0, 2\n"
		"\tvmrglw 5, 0, 2\n"
		"\tvmrghw 6, 1, 3\n"
		"\tvmrglw 7, 1, 3\n"
		/* 00 20 01 21
		 * 02 22 03 23
		 * 10 30 11 31
		 * 12 32 13 33 */
		"\tvmrghw 0, 4, 6\n"
		"\tvmrglw 1, 4, 6\n"
		"\tvmrghw 2, 5, 7\n"
		"\tvmrglw 3, 5, 7\n"
	);

	/* store vectors */
	sl_altivec_store4_0(dest,dstr);
}

#define f32_addr(a,b,x) ((f32 *)((void *)(a) + (b)*(x)))

/* IMPL trans8x8_f32_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_f32_storage
void trans8x8_f32_a16_altivec(f32 *dest, f32 *src, int dstr, int sstr)
{
	trans4x4_f32_a16_altivec(dest,src,dstr,sstr);
	trans4x4_f32_a16_altivec(dest+4,f32_addr(src,sstr,4),dstr,sstr);
	trans4x4_f32_a16_altivec(f32_addr(dest,dstr,4),src+4,dstr,sstr);
	trans4x4_f32_a16_altivec(f32_addr(dest,dstr,4)+4,
			f32_addr(src,sstr,4)+4,dstr,sstr);
}
#endif

#endif


