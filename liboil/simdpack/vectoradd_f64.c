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


#define VECTORADD_DEFINE_IMPL(type)		\
static void vectoradd_ ## type ## _ref (	\
    type_ ## type *dest, int dstr,		\
    type_ ## type *src1, int sstr1,		\
    type_ ## type *src2, int sstr2,		\
    int n, type_ ## type v1, type_ ## type v2)	\
{						\
  int i;					\
  for(i=0;i<n;i++) {				\
    *OIL_OFFSET(dest,i*dstr) = v1 * *OIL_OFFSET(src1,i*sstr1) +	\
        v2 * *OIL_OFFSET(src2,i*sstr2);		\
  }						\
}						\
OIL_DEFINE_IMPL_REF (vectoradd_ ## type ## _ref, vectoradd_ ## type ## _class);

VECTORADD_DEFINE_IMPL (s8);
VECTORADD_DEFINE_IMPL (u8);
VECTORADD_DEFINE_IMPL (s16);
VECTORADD_DEFINE_IMPL (u16);
VECTORADD_DEFINE_IMPL (s32);
VECTORADD_DEFINE_IMPL (u32);
VECTORADD_DEFINE_IMPL (f32);
VECTORADD_DEFINE_IMPL (f64);




#ifdef TEST_vectoradd_f64
int TEST_vectoradd_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src1, *src2, *dest_ref, *dest_test;
	struct sl_profile_struct t;
	double v1, v2;

	src1 = sl_malloc_f64(N);
	src2 = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(vectoradd_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++){
			src1[i]=sl_rand_f64_0_1();
			src2[i]=sl_rand_f64_0_1();
		}
		v1 = sl_rand_f64_0_1();
		v2 = sl_rand_f64_0_1();

		vectoradd_f64_ref(dest_ref,src1,src2,N,v1,v2);
		sl_profile_start(t);
		vectoradd_f64_FUNC(dest_test,src1,src2,N,v1,v2);
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %g %g\n",i,src1[i],dest_ref[i],
						dest_test[i]);
			}
		}
	}

	sl_free(src1);
	sl_free(src2);
	sl_free(dest_ref);
	sl_free(dest_test);

	if(failures){
		printf("E: %d failures\n",failures);
	}

	sl_profile_print(t);

	return failures;
}
#endif

