/*
 * LIBOIL - Library of Optimized Inner Loops
 * Copyright (c) 2001,2002,2004 David A. Schleef <ds@schleef.org>
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
Kernel: conv8x8_s16x8_s8
Description: round source value to nearest integer and convert to s16.

*/

#ifndef _conv8x8_s16x8_s8_h_
#define _conv8x8_s16x8_s8_h_

#include <math.h>
#include <sl_types.h>

#include <sl_block8x8.h>


/* storage class */
#ifndef SL_conv8x8_s16x8_s8_storage
 #ifdef SL_storage
  #define SL_conv8x8_s16x8_s8_storage SL_storage
 #else
  #define SL_conv8x8_s16x8_s8_storage static inline
 #endif
#endif

/* IMPL conv8x8_s16x8_s8_ref */
SL_conv8x8_s16x8_s8_storage
void conv8x8_s16x8_s8_ref(s8 *dest, s16 *src, int dstr, int sstr)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_s8(dest,dstr,i,j) =
				(block8x8_s16(src,sstr,i,j)+128)>>8;
		}
	}
}
#endif

