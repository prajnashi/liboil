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
Kernel: idct8x8_f64
Description: inverse discrete cosine transform on 8x8 block

XXX
*/

#ifndef _idct8x8_f64_h_
#define _idct8x8_f64_h_

#include <math.h>

#include <sl_types.h>
#include <sl_block8x8.h>

/* storage class */
#ifndef SL_idct8x8_f64_storage
 #ifdef SL_storage
  #define SL_idct8x8_f64_storage SL_storage
 #else
  #define SL_idct8x8_f64_storage static inline
 #endif
#endif


/* IMPL idct8x8_f64_ref */
SL_idct8x8_f64_storage
void idct8x8_f64_ref(f64 *dest, f64 *src, int dstr, int sstr)
{
	static f64 idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j,k,l;
	f64 tmp1,tmp2;

	if(!idct_coeff_init){
		f64 scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				idct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		idct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<8;k++){
				tmp2 = 0;
				for(l=0;l<8;l++){
					tmp2 += idct_coeff[j][l] *
						block8x8_f64(src,sstr,k,l);
				}
				tmp1 += idct_coeff[i][k] * tmp2;
			}
			block8x8_f64(dest,dstr,i,j) = tmp1;
		}
	}
}

/* IMPL idct8x8_f64_ref2 */
SL_idct8x8_f64_storage
void idct8x8_f64_ref2(f64 *dest, f64 *src, int dstr, int sstr)
{
	static f64 idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j,k;
	f64 x;
	f64 tmp[64];

	if(!idct_coeff_init){
		f64 scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				idct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		idct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			x = 0;
			for(k=0;k<8;k++){
				x += idct_coeff[j][k] *
					block8x8_f64(src,sstr,i,k);
			}
			tmp[8*i+j] = x;
		}
	}

	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			x = 0;
			for(k=0;k<8;k++){
				x += idct_coeff[i][k] * tmp[8*k + j];
			}
			block8x8_f64(dest,dstr,i,j) = x;
		}
	}
}

#define f64_addr(base,str,i) ((f64 *)((void *)(base) + (str)*(i)))

#include <idct8_f64.h>
/* IMPL idct8x8_f64_1d */
SL_idct8x8_f64_storage
void idct8x8_f64_1d(f64 *dest, f64 *src, int dstr, int sstr)
{
	int i;
	f64 tmp[64];

	for(i=0;i<8;i++){
		idct8_f64_fast(tmp + i*8, f64_addr(src,sstr,i), sizeof(f64), sizeof(f64));
	}
	for(i=0;i<8;i++){
		idct8_f64_fast(dest + i, tmp + i, dstr, 8*sizeof(f64));
	}
}


#endif


