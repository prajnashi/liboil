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


static void get8x8_f64_ref(double *dest, int dstr, double *src, int sstr,
    int x, int y)
{
	int i,j;

	for(i=0;i<8;i++){
		for(j=0;j<8;j++){
			OIL_GET (dest,dstr*i+j, double) = 
				OIL_GET (src,sstr*(i+y)+j+x, double);
		}
	}
}

OIL_DEFINE_IMPL_REF (get8x8_f64_ref, get8x8_f64_class);


/* This function needs help */
#ifdef TEST_get8x8_f64
int TEST_get8x8_f64(void)
{
	int i;
	int pass;
	int failures = 0;
	int x,y;
	f64 *src, *dest_ref, *dest_test;
	struct sl_profile_struct t;

	src = sl_malloc_f64(9*64);
	dest_ref = sl_malloc_f64(64);
	dest_test = sl_malloc_f64(64);
	
	sl_profile_init(t);
	srand(20020326);

	printf("I: " sl_stringify(get8x8_f64_FUNC) "\n");

	for(pass=0;pass<N_PASS;pass++){
		for(i=0;i<9*64;i++)src[i] = sl_rand_f64_0_1();
		x = sl_rand_s16_l4();
		y = sl_rand_s16_l4();

		get8x8_f64_ref(dest_test, src + 8*(8*3) + 8, 8*sizeof(f64), 8*sizeof(f64),
			x,y);
		sl_profile_start(t);
		get8x8_f64_FUNC(dest_ref, src + 8*(8*3) + 8, 8*sizeof(f64), 8*sizeof(f64),
			x,y);
		sl_profile_stop(t);

		for(i=0;i<64;i++){
			if(dest_test[i]!=dest_ref[i]){
				failures++;
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

