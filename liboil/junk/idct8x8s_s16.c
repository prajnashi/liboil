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
Kernel: idct8x8s_s16
Description: inverse discrete cosine transform on 8x8 block

Alternate scaling used by RTjpeg.
*/

#ifndef _idct8x8s_s16_h_
#define _idct8x8s_s16_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>

/* storage class */
#ifndef SL_idct8x8s_s16_storage
 #ifdef SL_storage
  #define SL_idct8x8s_s16_storage SL_storage
 #else
  #define SL_idct8x8s_s16_storage static inline
 #endif
#endif


/* extras */
#include <rtjpeg/idct8x8s_s16.h>


#include <idct8x8_f64.h>
#include <conv8x8_f64_s16.h>
/* IMPL idct8x8s_s16_ref */
SL_idct8x8s_s16_storage
void idct8x8s_s16_ref(s16 *dest, s16 *src, int dstr, int sstr)
{
	f64 s[64], d[64];
	const f64 scale[8] = {
		2.0/C0_7071,
		2.0/C0_9808,
		2.0/C0_9239,
		2.0/C0_8315,
		2.0/C0_7071,
		2.0/C0_5556,
		2.0/C0_3827,
		2.0/C0_1951,
	};
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_f64(s,8*sizeof(f64),i,j) =
				block8x8_s16(src,sstr,i,j);
		}
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			block8x8_f64(s,8*sizeof(f64),i,j) *= scale[i] * scale[j];
		}
	}

	idct8x8_f64(d,s,8*sizeof(f64),8*sizeof(f64));

	conv8x8_f64_s16(dest,d,dstr,8*sizeof(f64));
}
#endif

