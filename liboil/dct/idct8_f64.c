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
#include <liboil/dct/dct.h>
#include <math.h>

OIL_DEFINE_CLASS (idct8_f64, "double *d_8, int dstr, double *s_8, int sstr");

#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

static void idct8_f64_ref(double *dest, int dstr, double *src, int sstr)
{
	static double idct_coeff[8][8];
	static int idct_coeff_init = 0;
	int i,j;
	double x;

	if(!idct_coeff_init){
		double scale;

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
		x = 0;
		for(j=0;j<8;j++){
			x += idct_coeff[i][j] * OIL_GET (src, sstr*j, double);
		}
		OIL_GET (dest, dstr*i, double) = x;
	}
}

OIL_DEFINE_IMPL_REF (idct8_f64_ref, idct8_f64);


static void idct8_f64_fastx(double *dest, int dstr, double *src, int sstr)
{
	double s07, s16, s25, s34;
	double d07, d16, d25, d34;
	double ss07s34, ss16s25;
	double ds07s34, ds16s25;

	ss07s34 = C0_7071*(OIL_GET(src,sstr*0, double) + OIL_GET(src,sstr*4, double));
	ss16s25 = C0_7071*(OIL_GET(src,sstr*0, double) - OIL_GET(src,sstr*4, double));

	ds07s34 = C0_9239* OIL_GET(src,sstr*2, double) + C0_3827* OIL_GET(src,sstr*6, double);
	ds16s25 = C0_3827* OIL_GET(src,sstr*2, double) - C0_9239* OIL_GET(src,sstr*6, double);

	s07 = ss07s34 + ds07s34;
	s34 = ss07s34 - ds07s34;

	s16 = ss16s25 + ds16s25;
	s25 = ss16s25 - ds16s25;

	d07 = C0_9808* OIL_GET(src,sstr*1, double) + C0_8315* OIL_GET(src,sstr*3, double)
		+ C0_5556* OIL_GET(src,sstr*5, double) + C0_1951* OIL_GET(src,sstr*7, double);
	d16 = C0_8315* OIL_GET(src,sstr*1, double) - C0_1951* OIL_GET(src,sstr*3, double)
		- C0_9808* OIL_GET(src,sstr*5, double) - C0_5556* OIL_GET(src,sstr*7, double);
	d25 = C0_5556* OIL_GET(src,sstr*1, double) - C0_9808* OIL_GET(src,sstr*3, double)
		+ C0_1951* OIL_GET(src,sstr*5, double) + C0_8315* OIL_GET(src,sstr*7, double);
	d34 = C0_1951* OIL_GET(src,sstr*1, double) - C0_5556* OIL_GET(src,sstr*3, double)
		+ C0_8315* OIL_GET(src,sstr*5, double) - C0_9808* OIL_GET(src,sstr*7, double);

	OIL_GET(dest,dstr*0, double) = 0.5 * (s07 + d07);
	OIL_GET(dest,dstr*1, double) = 0.5 * (s16 + d16);
	OIL_GET(dest,dstr*2, double) = 0.5 * (s25 + d25);
	OIL_GET(dest,dstr*3, double) = 0.5 * (s34 + d34);
	OIL_GET(dest,dstr*4, double) = 0.5 * (s34 - d34);
	OIL_GET(dest,dstr*5, double) = 0.5 * (s25 - d25);
	OIL_GET(dest,dstr*6, double) = 0.5 * (s16 - d16);
	OIL_GET(dest,dstr*7, double) = 0.5 * (s07 - d07);

}

OIL_DEFINE_IMPL (idct8_f64_fastx, idct8_f64);


#ifdef TEST_idct8_f64
int TEST_idct8_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	double *src, *dest_ref, *dest_test;
	double sad;
	double sad_sum;
	double sad_max;
	struct sl_profile_struct t;

	src = sl_malloc_f64(8);
	dest_ref = sl_malloc_f64(8);
	dest_test = sl_malloc_f64(8);
	
	sl_profile_init(t);
	srand(20020306);

	sad_sum = 0;
	sad_max = 0;

	printf("I: " sl_stringify(idct8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<8;i++)src[i] = sl_rand_f64_0_1();

		//block8_dump(src);

		idct8_f64_ref(dest_test, src, 8, 8);
		//block8_dump(dest_test);

		sl_profile_start(t);
		idct8_f64_FUNC(dest_ref, src, 8, 8);
		sl_profile_stop(t);
		//block8_dump(dest_ref);

		sad = 0;
		for(i=0;i<8;i++)sad += fabs(dest_test[i] - dest_ref[i]);
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

