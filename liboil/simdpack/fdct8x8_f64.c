/* liboil - Library of Optimized Inner Loops
 * Copyright (C) 2003  David A. Schleef <ds@schleef.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser General
 * Public License as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <liboil/liboilfunction.h>
#include <liboil/simdpack/simdpack.h>
#include <math.h>




static void fdct8x8_f64_ref(double *dest, int dstr, double *src, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j,k,l;
	double tmp1,tmp2;

	if(!fdct_coeff_init){
		double scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				fdct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		fdct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			tmp1 = 0;
			for(k=0;k<8;k++){
				tmp2 = 0;
				for(l=0;l<8;l++){
					tmp2 += fdct_coeff[l][j] *
					  OIL_GET (src, sstr*k + l, double);
				}
				tmp1 += fdct_coeff[k][i] * tmp2;
			}
			OIL_GET (dest, dstr*i + j, double) = tmp1;
		}
	}
}

OIL_DEFINE_IMPL_REF (fdct8x8_f64_ref, fdct8x8_f64_class);

static void
fdct8x8_f64_ref2(double *dest, int dstr, double *src, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j,k;
	double x;
	double tmp[64];

	if(!fdct_coeff_init){
		double scale;

		for(i=0;i<8;i++){
			scale = (i==0) ? sqrt(0.125) : 0.5;
			for(j=0;j<8;j++){
				fdct_coeff[j][i] = scale *
					cos((M_PI/8)*i*(j+0.5));
			}
		}
		fdct_coeff_init = 1;
	}

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			x = 0;
			for(k=0;k<8;k++){
				x += fdct_coeff[k][j] *
				  OIL_GET (src, sstr*i + k, double);
			}
			tmp[8*i+j] = x;
		}
	}

	for(j=0;j<8;j++){
		for(i=0;i<8;i++){
			x = 0;
			for(k=0;k<8;k++){
				x += fdct_coeff[k][i] * tmp[8*k + j];
			}
			OIL_GET (dest,dstr*i+j, double) = x;
		}
	}
}

OIL_DEFINE_IMPL (fdct8x8_f64_ref2, fdct8x8_f64_class);


static void
fdct8x8_f64_1d (double *dest, int dstr, double *src, int sstr)
{
	int i;
	double tmp[64];

	for(i=0;i<8;i++){
		fdct8_f64(tmp + i*8, sizeof(double), OIL_OFFSET(src,sstr*i),
		    sizeof(double));
	}

	for(i=0;i<8;i++){
		fdct8_f64(dest + i, dstr, tmp + i, 8*sizeof(double));
	}
}

OIL_DEFINE_IMPL (fdct8x8_f64_1d, fdct8x8_f64_class);


#ifdef TEST_fdct8x8_f64
int TEST_fdct8x8_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	double *src, *dest_ref, *dest_test;
	double sad;
	double sad_sum;
	double sad_max;
	struct sl_profile_struct t;

	src = sl_malloc_f64(64);
	dest_ref = sl_malloc_f64(64);
	dest_test = sl_malloc_f64(64);
	
	sl_profile_init(t);
	srand(20020306);

	sad_sum = 0;
	sad_max = 0;

	printf("I: " sl_stringify(fdct8x8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src[i] = sl_rand_f64_0_1();

		fdct8x8_f64_ref(dest_test, src, 8, 8);
		sl_profile_start(t);
		fdct8x8_f64_FUNC(dest_ref, src, 8, 8);
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

