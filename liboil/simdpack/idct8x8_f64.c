/* inverse discrete cosine transform on 8x8 block
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


#ifdef TEST_idct8x8_f64
int TEST_idct8x8_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src, *dest_ref, *dest_test;
	f64 sad;
	f64 sad_sum;
	f64 sad_max;
	struct sl_profile_struct t;

	src = sl_malloc_f64(64);
	dest_ref = sl_malloc_f64(64);
	dest_test = sl_malloc_f64(64);
	
	sl_profile_init(t);
	srand(20020306);

	sad_sum = 0;
	sad_max = 0;

	printf("I: " sl_stringify(idct8x8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src[i] = sl_rand_f64_0_1();

		idct8x8_f64_ref(dest_test, src, 8, 8);
		sl_profile_start(t);
		idct8x8_f64_FUNC(dest_ref, src, 8, 8);
		sl_profile_stop(t);

		sad = 0;
		for(i=0;i<64;i++)sad += fabs(dest_test[i] - dest_ref[i]);
		if(sad_max<sad)sad_max = sad;
		sad_sum += sad;
		if(sad >= 1.0){
			failures++;
		}
	}
	printf("sad average: %g\n",sad_sum/N_PASS);
	printf("sad max: %g\n",sad_max);

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

