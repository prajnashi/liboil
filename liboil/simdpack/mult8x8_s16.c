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


OIL_DEFINE_CLASS (mult8x8_s16, 
    "int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1, int sstr2");

static void
mult8x8_s16_ref(int16_t *dest, int16_t *src1, int16_t *src2, int dstr, int sstr1,
    int sstr2)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			OIL_GET(dest,dstr*i+j, int16_t) =
				OIL_GET(src1,sstr1*i+j, int16_t) *
				OIL_GET(src1,sstr1*i+j, int16_t);
		}
	}
}

OIL_DEFINE_IMPL_REF (mult8x8_s16_ref, mult8x8_s16_class);

#ifdef TEST_mult8x8_s16
int TEST_mult8x8_s16(void)
{
	int i;
	int pass;
	int failures = 0;
	int16_t *src1, *src2, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src1 = sl_malloc_s16(64);
	src2 = sl_malloc_s16(64);
	dest_ref = sl_malloc_s16(64);
	dest_test = sl_malloc_s16(64);
	
	sl_profile_init(t);
	srand(20020401);

	printf("I: " sl_stringify(mult8x8_s16_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<64;i++)src1[i] = sl_rand_s16_l8();
		for(i=0;i<64;i++)src2[i] = sl_rand_s16_l8();

		mult8x8_s16_ref(dest_ref, src1, src2, 8*sizeof(s16),
			8*sizeof(s16), 8*sizeof(s16));
		sl_profile_start(t);
		mult8x8_s16_FUNC(dest_test, src1, src2, 8*sizeof(s16),
			8*sizeof(s16), 8*sizeof(s16));
		sl_profile_stop(t);

		for(i=0;i<64;i++){
			if(dest_test[i] != dest_ref[i]){
				failures++;
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

