/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003 David A. Schleef <ds@schleef.org>
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>

static const unsigned char zigzag_order[64] = {
	0,
	8, 1,
	2, 9, 16,
	24, 17, 10, 3, 
	4, 11, 18, 25, 32,
	40, 33, 26, 19, 12, 5, 
	6, 13, 20, 27, 34, 41, 48,
	56, 49, 42, 35, 28, 21, 14, 7,
	15, 22, 29, 36, 43, 50, 57,
	58, 51, 44, 37, 30, 23,
	31, 38, 45, 52, 59,
	60, 53, 46, 39,
	47, 54, 61,
	62, 55,
	63
};

static void zigzag8x8_s16_ref(int16_t *dest, int16_t *src, int sstr)
{
	int i,j;
	unsigned int z;

	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			z = zigzag_order[j*8+i];
			dest[j*8+i] = OIL_GET(src, sstr*(z>>3)+(z&7), int16_t);
		}
	}
}
OIL_DEFINE_IMPL (zigzag8x8_s16_ref, zigzag8x8_s16_class);


#ifdef HAVE_CPU_POWERPC
/* 00 indicates that the element can't be handled by vperm, and needs
 * to be fixed up later. */
u8 mangle[128] __attribute__ ((__aligned__ (16))) = {
	00,00, 2, 3, 0, 1,16,17,18,19,20,21,00,00,00,00, /* 1, 2 */
	 2, 3, 0, 1,16,17,18,19,20,21,22,23,24,25,00,00, /* 3, 4 */
	 8, 9, 6, 7, 4, 5, 2, 3, 0, 1,16,17,18,19,20,21, /* 5, 6 */
	 6, 7, 8, 9,10,11,12,13,30,31,28,29,26,27,24,25, /* 6, 7 */
	 6, 7, 4, 5, 2, 3, 0, 1,18,19,20,21,22,23,24,25, /* 7, 0 */
	10,11,12,13,14,15,30,31,28,29,26,27,24,25,22,23, /* 0, 1 */
	00,00, 6, 7, 8, 9,10,11,12,13,14,15,30,31,28,29, /* 2, 3 */
	00,00,00,00,10,11,12,13,14,15,30,31,28,29,00,00, /* 4, 5 */
};

/* IMPL zigzag8x8_s16_a16_altivec defined(SIMDPACK_USE_ALTIVEC) */
SL_zigzag8x8_s16_storage
void zigzag8x8_s16_a16_altivec(int16_t *dest, int16_t *src, int sstr)
{
	sl_altivec_load8_0(src,sstr);

	/* "slide" vectors to right */
	__asm__ __volatile__(
		"vsldoi 0,0,0,0\n"
		"vsldoi 1,1,1,14\n"
		"vsldoi 2,2,2,12\n"
		"vsldoi 3,3,3,10\n"
		"vsldoi 4,4,4,8\n"
		"vsldoi 5,5,5,6\n"
		"vsldoi 6,6,6,4\n"
		"vsldoi 7,7,7,2\n"
	);

	/* transpose */
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

	sl_altivec_load8_8(mangle,16);

	__asm__ __volatile__(
		"\n"
		"\tvperm 16,1,2,8\n"
		"\tvperm 17,3,4,9\n"
		"\tvperm 18,5,6,10\n"
		"\tvperm 19,6,7,11\n"
		"\tvperm 20,7,0,12\n"
		"\tvperm 21,0,1,13\n"
		"\tvperm 22,2,3,14\n"
		"\tvperm 23,4,5,15\n"
	);

	sl_altivec_store8_16(dest,16);

	/* fix up the elements that were missed */

	block8x8_s16(dest,16,0,0) = block8x8_s16(src,sstr,0,0);
	block8x8_s16(dest,16,0,6) = block8x8_s16(src,sstr,3,0);
	block8x8_s16(dest,16,0,7) = block8x8_s16(src,sstr,2,1);
	block8x8_s16(dest,16,1,7) = block8x8_s16(src,sstr,5,0);

	block8x8_s16(dest,16,6,0) = block8x8_s16(src,sstr,2,7);
	block8x8_s16(dest,16,7,0) = block8x8_s16(src,sstr,5,6);
	block8x8_s16(dest,16,7,1) = block8x8_s16(src,sstr,4,7);
	block8x8_s16(dest,16,7,7) = block8x8_s16(src,sstr,7,7);
}
#endif


