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

OIL_DEFINE_CLASS_X (fdct8_f64, "double *dest, double *src, int dstr, int sstr");

#define C0_9808 0.980785280
#define C0_9239 0.923879532
#define C0_8315 0.831469612
#define C0_7071 0.707106781
#define C0_5556 0.555570233
#define C0_3827 0.382683432
#define C0_1951 0.195090322

static void
fdct8_f64_ref (double *dest, double *src, int dstr, int sstr)
{
	static double fdct_coeff[8][8];
	static int fdct_coeff_init = 0;
	int i,j;
	double x;

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
		x = 0;
		for(j=0;j<8;j++){
			x += fdct_coeff[j][i] * OIL_GET(src,sstr*j, double);
		}
		OIL_GET(dest,dstr*i, double) = x;
	}
}

OIL_DEFINE_IMPL_REF (fdct8_f64_ref, fdct8_f64_class);

/*
 * This algorithm is roughly similar to a Fast-Fourier transform,
 * taking advantage of the symmeties of the base vectors.  For
 * reference, the base vectors are (horizontally):
 *
 * 0: 1.0000  1.0000  1.0000  1.0000  1.0000  1.0000  1.0000  1.0000 
 * 1: 0.9808  0.8315  0.5556  0.1951 -0.1951 -0.5556 -0.8315 -0.9808 
 * 2: 0.9239  0.3827 -0.3827 -0.9239 -0.9239 -0.3827  0.3827  0.9239 
 * 3: 0.8315 -0.1951 -0.9808 -0.5556  0.5556  0.9808  0.1951 -0.8315 
 * 4: 0.7071 -0.7071 -0.7071  0.7071  0.7071 -0.7071 -0.7071  0.7071 
 * 5: 0.5556 -0.9808  0.1951  0.8315 -0.8315 -0.1951  0.9808 -0.5556 
 * 6: 0.3827 -0.9239  0.9239 -0.3827 -0.3827  0.9239 -0.9239  0.3827 
 * 7: 0.1951 -0.5556  0.8315 -0.9808  0.9808 -0.8315  0.5556 -0.1951 
 *
 * The symmetries of note: 
 *  - even vectors are symmetric around 4 (the middle)
 *  - odd vectors are antisymmetric around 4
 *  - 0,4 are symmertic around 2 and 6
 *  - 2,6 are antisymmetic around 2 and 6
 *
 * f0 = (x0 + x7) + (x1 + x6) + (x2 + x5) + (x3 + x4);
 * f1 = 0.9808*(x0 - x7) + 0.8315*(x1 - x6) + 0.5556*(x2 - x5) + 0.1951*(x3 - x4)
 * f2 = 0.9239*(x0 + x7) + 0.3827*(x1 + x6) - 0.3827*(x2 + x5) - 0.9239*(x3 + x4)
 * f3 = 0.8315*(x0 - x7) - 0.1951*(x1 - x6) - 0.9808*(x2 - x5) - 0.5556*(x3 - x4)
 * f4 = 0.7071*((x0 + x7) - (x1 + x6) - (x2 + x5) + (x3 + x4))
 * f5 = 0.5556*(x0 - x7) - 0.9808*(x1 - x6) + 0.1951*(x2 - x5) + 0.8315*(x3 - x4)
 * f6 = 0.3827*(x0 + x7) - 0.9239*(x1 + x6) + 0.9239*(x2 + x5) - 0.3827*(x3 + x4)
 * f7 = 0.1951*(x0 - x7) - 0.5556*(x1 - x6) + 0.8315*(x2 - x5) - 0.9808*(x3 - x4)
 *
 * The even vectors can be further simplified:
 *
 * f0 = ((x0 + x7) + (x3 + x4)) + ((x1 + x6) + (x2 + x5));
 * f2 = 0.9239*((x0 + x7) - (x3 + x4)) + 0.3827*((x1 + x6) - (x2 + x5))
 * f4 = 0.7071*((x0 + x7) + (x3 + x4)) - 0.7071*((x1 + x6) + (x2 + x5))
 * f6 = 0.3827*((x0 + x7) - (x3 + x4)) - 0.9239*((x1 + x6) - (x2 + x5))
 *
 * Some implementations move some of the normalization to a later
 * stage of processing, saving a few multiplies which get absorbed
 * into later multiplies.  However, this incurs a bit of error in
 * the integer versions of this function.  Also, if the CPU has a
 * multiply-and-add function, you don't gain anything.
 */

static void
fdct8_f64_fast(double *dest, double *src, int dstr, int sstr)
{
	double s07, s16, s25, s34;
	double d07, d16, d25, d34;
	double ss07s34, ds07s34, ss16s25, ds16s25;

	s07 = OIL_GET(src,sstr*0,double) + OIL_GET(src,sstr*7,double);
	d07 = OIL_GET(src,sstr*0,double) - OIL_GET(src,sstr*7,double);
	s16 = OIL_GET(src,sstr*1,double) + OIL_GET(src,sstr*6,double);
	d16 = OIL_GET(src,sstr*1,double) - OIL_GET(src,sstr*6,double);
	s25 = OIL_GET(src,sstr*2,double) + OIL_GET(src,sstr*5,double);
	d25 = OIL_GET(src,sstr*2,double) - OIL_GET(src,sstr*5,double);
	s34 = OIL_GET(src,sstr*3,double) + OIL_GET(src,sstr*4,double);
	d34 = OIL_GET(src,sstr*3,double) - OIL_GET(src,sstr*4,double);

	ss07s34 = s07 + s34;
	ds07s34 = s07 - s34;
	ss16s25 = s16 + s25;
	ds16s25 = s16 - s25;

	OIL_GET(dest,dstr*0,double) = 0.5*C0_7071*(ss07s34 + ss16s25);
	OIL_GET(dest,dstr*2,double) = 0.5*(C0_9239*ds07s34 + C0_3827*ds16s25);
	OIL_GET(dest,dstr*4,double) = 0.5*C0_7071*(ss07s34 - ss16s25);
	OIL_GET(dest,dstr*6,double) = 0.5*(C0_3827*ds07s34 - C0_9239*ds16s25);

	OIL_GET(dest,dstr*1,double) = 0.5*(C0_9808*d07 + C0_8315*d16
			+ C0_5556*d25 + C0_1951*d34);
	OIL_GET(dest,dstr*3,double) = 0.5*(C0_8315*d07 - C0_1951*d16
			- C0_9808*d25 - C0_5556*d34);
	OIL_GET(dest,dstr*5,double) = 0.5*(C0_5556*d07 - C0_9808*d16
			+ C0_1951*d25 + C0_8315*d34);
	OIL_GET(dest,dstr*7,double) = 0.5*(C0_1951*d07 - C0_5556*d16
			+ C0_8315*d25 - C0_9808*d34);
 
#if 0
	z1 = (ds1  tmp12 + tmp13) * 0.707106781;
	OIL_GET(dest,dstr*2,double) = (tmp13 + z1)*(0.25*M_SQRT2)*0.765366864;
	OIL_GET(dest,dstr*6,double) = (tmp13 - z1)*(0.25*M_SQRT2)*1.847759066;

	tmp10 = tmp4 + tmp5;
	tmp11 = tmp5 + tmp6;
	tmp12 = tmp6 + tmp7;

	z5 = (tmp10 - tmp12) * 0.382683433;
	z2 = 0.541196100 * tmp10 + z5;
	z4 = 1.306562965 * tmp12 + z5;
	z3 = tmp11 * 0.707106781;

	z11 = tmp7 + z3;
	z13 = tmp7 - z3;

	OIL_GET(dest,dstr*5,double) = (z13 + z2)*(0.25*M_SQRT2)*1.2728;
	OIL_GET(dest,dstr*3,double) = (z13 - z2)*(0.25*M_SQRT2)*0.8504;
	OIL_GET(dest,dstr*1,double) = (z11 + z4)*(0.25*M_SQRT2)*0.7209;
	OIL_GET(dest,dstr*7,double) = (z11 - z4)*(0.25*M_SQRT2)*3.6245;
#endif
}
OIL_DEFINE_IMPL (fdct8_f64_fast, fdct8_f64_class);




#ifdef TEST_fdct8_f64
int TEST_fdct8_f64(void)
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

	printf("I: " sl_stringify(fdct8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<8;i++)src[i] = sl_rand_f64_0_1();

		//block8_dump(src);

		fdct8_f64_ref(dest_test, src, 8, 8);
		//block8_dump(dest_test);

		sl_profile_start(t);
		fdct8_f64_FUNC(dest_ref, src, 8, 8);
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

