/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2003,2004 David A. Schleef <ds@schleef.org>
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
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm, clip_s16);

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
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm2, clip_s16);

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
OIL_DEFINE_IMPL_ASM (clip_s16_ppcasm3, clip_s16);


