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
Kernel: trans8x8_s16
Description: transpose 8x8 block

XXX
*/

#ifndef _trans8x8_s16_h_
#define _trans8x8_s16_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>
#include <sl_altivec.h>

/* storage class */
#ifndef SL_trans8x8_s16_storage
 #ifdef SL_storage
  #define SL_trans8x8_s16_storage SL_storage
 #else
  #define SL_trans8x8_s16_storage static inline
 #endif
#endif


/* IMPL trans8x8_s16_ref */
SL_trans8x8_s16_storage
void trans8x8_s16_ref(s16 *dest, s16 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(dest,dstr,i,j) =
				block8x8_s16(src,sstr,j,i);
		}
	}
}


#ifdef SIMDPACK_USE_ALTIVEC
/* IMPL trans8x8_s16_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_s16_storage
void trans8x8_s16_a16_altivec(s16 *dest, s16 *src, int dstr, int sstr)
{
	sl_altivec_load8_0(src,sstr);

	__asm__ __volatile__(
		"vmrghh    8, 0, 4\n"
		"\tvmrglh  9, 0, 4\n"
		"\tvmrghh 10, 1, 5\n"
		"\tvmrglh 11, 1, 5\n"
		"\tvmrghh 12, 2, 6\n"
		"\tvmrglh 13, 2, 6\n"
		"\tvmrghh 14, 3, 7\n"
		"\tvmrglh 15, 3, 7\n"

		"\tvmrghh 16,  8, 12\n"
		"\tvmrglh 17,  8, 12\n"
		"\tvmrghh 18,  9, 13\n"
		"\tvmrglh 19,  9, 13\n"
		"\tvmrghh 20, 10, 14\n"
		"\tvmrglh 21, 10, 14\n"
		"\tvmrghh 22, 11, 15\n"
		"\tvmrglh 23, 11, 15\n"

		"\tvmrghh  0, 16, 20\n"
		"\tvmrglh  1, 16, 20\n"
		"\tvmrghh  2, 17, 21\n"
		"\tvmrglh  3, 17, 21\n"
		"\tvmrghh  4, 18, 22\n"
		"\tvmrglh  5, 18, 22\n"
		"\tvmrghh  6, 19, 23\n"
		"\tvmrglh  7, 19, 23\n"
	);

	sl_altivec_store8_0(dest,dstr);
}
#endif

#ifdef SIMDPACK_USE_ALTIVEC
/* IMPL trans8x8_s16_altivecwrap defined(SIMDPACK_USE_ALTIVEC) */
SL_trans8x8_s16_storage
void trans8x8_s16_altivecwrap(s16 *dest, s16 *src, int dstr, int sstr)
{
	static s16 s[64] __attribute__ ((__aligned__ (16)));
	static s16 d[64] __attribute__ ((__aligned__ (16)));
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(s,16,i,j) = block8x8_s16(src,sstr,i,j);
		}       
	}       

	trans8x8_s16_a16_altivec(d,s,16,16);

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s16(dest,dstr,i,j) = block8x8_s16(d,16,i,j);
		}       
	}

}
#endif

#endif

