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


#define MULTSUM_DEFINE_REF(type)	\
static void multsum_ ## type ## _ref(	\
    type_ ## type *dest,		\
    type_ ## type *src1, int sstr1,	\
    type_ ## type *src2, int sstr2,	\
    int n)				\
{					\
  int i;				\
  double sum = 0;			\
  double errsum = 0;			\
  for(i=0;i<n;i++){			\
    type_ ## type x;                    \
    type_ ## type tmp;                  \
    x = OIL_GET(src1,sstr1*i,type_ ## type) * OIL_GET(src2,sstr2*i,type_ ## type);		\
    tmp = sum;				\
    sum += x;				\
    errsum += (tmp - sum) + x;		\
  }					\
  *dest = sum + errsum;			\
}					\
OIL_DEFINE_IMPL_REF (multsum_ ## type ## _ref, multsum_ ## type ## _class); \
OIL_DEFINE_CLASS_X (multsum_ ## type, \
    "type_" #type " *dest, "		\
    "type_" #type " *src1, int sstr1, "	\
    "type_" #type " *src2, int sstr2, "	\
    "int n")

MULTSUM_DEFINE_REF(f32);
MULTSUM_DEFINE_REF(f64);


static void multsum_f32_unroll2 (float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
  int i;
  double sum1 = 0;
  double sum2 = 0;

  if (n&1) {
    sum1 += OIL_GET(src1,0, float) * OIL_GET(src2,0, float);
  }
  OIL_INCREMENT (src1, sstr1);
  OIL_INCREMENT (src2, sstr2);
  for(i=0;i<n;i++){
    sum1 += OIL_GET(src1,sstr1*i, float) * OIL_GET(src2,sstr2*i, float);
    sum2 += OIL_GET(src1,sstr1*i, float) * OIL_GET(src2,sstr2*i, float);
  }

  *dest = sum1 + sum2;
}
OIL_DEFINE_IMPL (multsum_f32_unroll2, multsum_f32_class);

#ifdef HAVE_CPU_POWERPC
/* This could probably be improved by unrolling */
static void multsum_f32_ppcasm(float *dest, float *src1, int sstr1,
    float *src2, int sstr2, int n)
{
	asm __volatile__("\n"
		"	lfs 0, 0(%0)\n"
		"	lfs 1, 0(%1)\n"
		"	fmuls 2, 0, 1\n"
		"	addi %2, %2, -1\n"
		"	mtctr %2\n"
		"1:	lfsu 0, 4(%0)\n"
		"	lfsu 1, 4(%1)\n"
		"	fmadds 2,0,1,2\n"
		"	bdnz 1b\n"
		"	stfs 2, 0(%3)\n"
	: "+b" (src1), "+b" (src2), "+b" (n)
	: "b" (dest)
	: "32", "33", "34", "ctr");
}
#endif



#ifdef TEST_multsum_f32
int TEST_multsum_f32(void)
{
	int i;
	int pass;
	int failures = 0;
	f32 *src1, *src2, *dest_test, *dest_ref;
	struct sl_profile_struct t;

	src1 = sl_malloc_f32(N);
	src2 = sl_malloc_f32(N);
	dest_test = sl_malloc_f32(1);
	dest_ref = sl_malloc_f32(1);

	sl_profile_init(t);
	srand(20020305);

	printf("I: " sl_stringify(multsum_f32_FUNC) "\n");
	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src1[i] = sl_rand_f32_0_1();
			src2[i] = sl_rand_f32_0_1();
		}
		multsum_f32_ref(dest_ref,src1,src2,N);
		sl_profile_start(t);
		multsum_f32_FUNC(dest_test,src1,src2,N);
		sl_profile_stop(t);
		if(!sl_equal_f32(dest_test[0],dest_ref[0])){
			printf("%d %g %g %g %g\n",pass,src1[0],src2[0],
				dest_ref[0],dest_test[0]);
			failures++;
		}
	}

	sl_free(src1);
	sl_free(src2);
	sl_free(dest_test);
	sl_free(dest_ref);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

