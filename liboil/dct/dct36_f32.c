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

OIL_DEFINE_CLASS(dct36_f32, "float *dest, int dstr, float *src, int sstr, int n");

static void
dct36_f32_ref(float *dest, int dstr, float *src, int sstr, int n)
{
  int i;
  for(i=0;i<n;i++){
    dest[i] = src[i];
  }
}

OIL_DEFINE_IMPL_REF (dct36_f32_ref, dct36_f32);

#ifdef TEST_dct36_f32
int TEST_dct36_f32(void)
{
	int i;
	int pass;
	int failures = 0;
	f64 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_f64(N);
	dest_ref = sl_malloc_f64(N);
	dest_test = sl_malloc_f64(N);

	sl_profile_init(t);
	srand(20021001);

	printf("I: " sl_stringify(dct36_f32_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<N;i++)src[i]=sl_rand_f64_s16();

		dct36_f32_ref(dest_ref,src,N);
		sl_profile_start(t);
		dct36_f32_FUNC(dest_test,src,N);
		sl_profile_stop(t);

		for(i=0;i<N;i++){
			if(dest_test[i] != dest_ref[i]){
				printf("%d %g %g %g\n",i,src[i],dest_ref[i],
						dest_test[i]);
			}
		}
	}

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

